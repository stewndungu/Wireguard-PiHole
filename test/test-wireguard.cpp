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
  string command = "docker exec wireguard hostname -i";

  return trim(sendCommand(command));
}


int main()
{
// Grabbing the ip for pihole
  string wireguard_ip = retrieveIP();
  if(wireguard_ip == "ERROR")
  {
    cout<< " ------- ERROR in reading Wireguard IP  -----" << endl;
    return 1;
  }

}
