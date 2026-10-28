#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>

using namespace std;

int connection_Pihole()
{
  system("cd container_pihole_vpn");
  int result = system("docker compose up -d pihole");
  return result;
}

  string sendCommand(string& cmd)
  {
    /*
      I create a variable pipe that uses popen to generate a separate background
      It runs the process and reads its stdout
    */
    FILE* pipe = popen(cmd.c_str(), "r");

    char buffer[128];
    string result="";
    if(!pipe)
    {
      return " -- ERROR in reading OUTPUT  -----" ;
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


string retrieveIP()
{
  //get ip address for pihole
  string command = "docker exec pihole hostname -1";

  return sendCommand(command);
}


string requestsTest(string& pihole_ip, string& url)
{ 
  // TODO: need to finish the check from result
  // Idea is it will use dig to send a request and depending on a response we send a message bacl
  string cmd = "dig @" + pihole_ip + " " + url;
  return sendCommand(cmd);
}

int main()
{
  // Checking the connection to the pihole container
  int connection = connection_Pihole();
  if(connection != 0)
  {
    cout<< "Error in standing up the pihole container" << endl;
    
    return 0;
  }
  else
  {
    cout << "--- Pi Hole stood up --------"<< endl;
  }

  // once gauging the ip for pihole
  string pihole_ip = retrieveIP();
  if(pihole_ip == "ERROR")
  {
    cout<< " -- ERROR in reading OUTPUT  -----" << endl;
    return 0;
  }

  vector<string> good_urls= {"google.com", "github.com"};
  vector<string> bad_urls= {"aws-update.net","12724.xyz"};

  //Good tests
  for(string& url : good_urls)
    {
       // TODO: need to finish the check from result
      string result = requestsTest(pihole_ip,url);

      if(result == "ERROR")
      {
          cout<< " -- ERROR in reading OUTPUT  -----" << endl;
          return 0;
      }
      
      if(result == "0.0.0.0")
      {
        cout<< url << " : error -  blocked";
        return 0;
      }
      else{
        cout<< url << " :  Not blocked";
        
      }
      
    }

  //Bad tests
  for(string& url : bad_urls)
  {
    // TODO: need to finish the check from result
    string result = requestsTest(pihole_ip, url);

    if(result == "0.0.0.0")
    {
     
        cout<< url << " : blocked";
       
    }
    else{
        cout << url << " : Not blocked";
        return 0;
    }
  }

  return 0;
}
