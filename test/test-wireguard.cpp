#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cctype>
using namespace std;

string sendCommand(string& cmd)
{
    /*
      I create a variable pipe that uses popen to generate a separate background
      It runs the process and reads its stdout
    */
    
    FILE* pipe { popen(cmd.c_str(), "r") };

    char buffer[128];
    string result="";
    if(!pipe)
    {
      return "ERROR" ;
    }

    /*
      The stdout in pipe goes into buffer as much as it can
        until it hits nullptr
        each time, it takes buffer's stdout into result
    */
    while(fgets(buffer,sizeof(buffer),pipe) != nullptr)
    {
      result += buffer;
    }


    //close the variable pipe which closes the background process
    pclose(pipe);
    return result;
  }

string trim(string value)
{
  const string whitespace = " \t\n\r\f\v";
  size_t first = value.find_first_not_of(whitespace);
  if(first == string::npos)
  {
    return "";
  }

  size_t last = value.find_last_not_of(whitespace);
  return value.substr(first, last - first + 1);
}


/*
  nslookup prints a whole block, e.g.
    Server:   172.18.0.2
    Address:  172.18.0.2:53
    Name:     dns.0pengl.com
    Address:  0.0.0.0
  The first "Address" is the DNS server itself, so only take the
  first Address line that comes after "Name:" (the actual answer).
  Returns "" if there is no answer.
*/
string parseNslookup(const string& output)
{
  bool seenName = false;
  size_t start = 0;
  while(start < output.size())
  {
    size_t end = output.find('\n', start);
    if(end == string::npos) end = output.size();
    string line = trim(output.substr(start, end - start));
    start = end + 1;

    if(line.rfind("Name:", 0) == 0)
    {
      seenName = true;
    }
    else if(seenName && line.rfind("Address", 0) == 0)
    {
      size_t colon = line.find(':');
      if(colon != string::npos)
      {
        return trim(line.substr(colon + 1));
      }
    }
  }
  return "";
}

string retrieveIP()
{
  //get ip address for pihole
  string command { "docker exec pihole hostname -i" };

  return trim(sendCommand(command));
}



int main()
{
// Grabbing the ip for pihole
  string pihole_ip { retrieveIP() };
  if(pihole_ip == "ERROR")
  {
    cout<< " ------- ERROR in reading Pi-hole IP  -----" << endl;
    return EXIT_FAILURE;
  }


  //TODO Listening ports
  string listeningPortsCommand {"docker exec wireguard wg show wg0"};
  string listeningPortsCheck { trim(sendCommand(listeningPortsCommand))};
  if(listeningPortsCheck == "ERROR")
  {
    cout<< " ------- ERROR in reading Wireguard Listening Ports  -----" << endl;
    return EXIT_FAILURE;
  }
  else
  {
    cout<< "Wireguard Listening Ports: " << listeningPortsCheck << endl;
  }

  //TODO: Handshake
  string handshakeCommand {"	docker exec wg-client wg show wg0 latest-handshakes"};
  string handshakeCheck { trim(sendCommand(handshakeCommand))};
  if(handshakeCheck == "ERROR")
  {
    cout<< " ------- ERROR in reading Wireguard Handshake  -----" << endl;
    return EXIT_FAILURE;
  }

  // TODO: check packet Loss
  string packetCommand {"docker exec wg-client ping -c 3 -W 2 172.16.1.1"};
  string packetCheck { trim(sendCommand(packetCommand))};
  if(packetCheck == "ERROR")
  {
    cout<< " ------- ERROR in reading Wireguard Packet Loss  -----" << endl;
    return EXIT_FAILURE;
  }



  vector<string> good_urls {"google.com", "github.com"};
  vector<string> bad_urls{"dns.0pengl.com","12724.xyz"};



  //checking if good URLs are resolved correctly and bad URLs are blocked
  for(const auto& good : good_urls)
  {
    string command{"docker exec wg-client nslookup -type=A " + good + " " + pihole_ip};
    string result{ parseNslookup(sendCommand(command)) };

    
      if(result == "ERROR")
      {
          cout<< " ---------- ERROR in reading good URL  -----" << endl;
          return EXIT_FAILURE;
      }

      if(result.empty())
      {
        cout << "----" + good + " : DNS lookup returned no answer" + "-------" << endl;
        return EXIT_FAILURE;
      }
      
      if(result == "0.0.0.0")
      {
        cout<< "------- " + good + " : error -  blocked ---------"<<endl;
        cout<< " ---- It was blocked when it shouldn't have  -----" << endl;
        return EXIT_FAILURE;
      }
      else{
        cout<< good << " :  Not blocked"<<endl;
        
      }
  }

  for(const auto& bad : bad_urls)
  {
    string command{"docker exec wg-client nslookup -type=A " + bad + " " + pihole_ip};
    string result{ parseNslookup(sendCommand(command)) };

    if(result == "ERROR")
    {
        cout<< " ---------- ERROR in reading bad URL  -----" << endl;
        return EXIT_FAILURE;
    }

    if(result == "0.0.0.0")
    {

        cout<< bad << " : blocked"<<endl;
       
    }
    else if(result.empty())
    {
      cout << bad << " : DNS lookup returned no answer; cannot confirm it is blocked" << endl;
      return EXIT_FAILURE;
    }
    else{
        cout <<"-------- " + bad + " : Not blocked ----------"<<endl;
        cout<< " ---- It wasn't blocked when it should have  -----" << endl;
      return EXIT_FAILURE;
    }
  }
  

  return EXIT_SUCCESS;

}
