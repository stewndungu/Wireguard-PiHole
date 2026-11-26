#include <iostream>
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


string retrieveIP()
{
  //get ip address for pihole
  string command { "docker exec wireguard hostname -i" };

  return trim(sendCommand(command));
}



int main()
{
// Grabbing the ip for pihole
  string wireguard_ip { retrieveIP() };
  if(wireguard_ip == "ERROR")
  {
    cout<< " ------- ERROR in reading Wireguard IP  -----" << endl;
    return EXIT_FAILURE;
  }

  vector<string> good_urls;
  vector<string> bad_urls;

  trim(sendCommand(string{"docker exec wireguard cat /config/peer_test/peer_test.conf"}));

  //checking if good URLs are resolved correctly and bad URLs are blocked
  for(const auto& good : good_urls)
  {
    string command{"docker exec wg-client nslookup" + " " + good + " " + wireguard_ip};
    string result{ trim(sendCommand(command)) };

    
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
    string command{"docker exec wg-client nslookup" + " " + bad + " " + wireguard_ip};
    string result{ trim(sendCommand(command)) };

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
