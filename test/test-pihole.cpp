#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

int connection_Pihole()
{
  system("cd container_pihole_vpn");
  int result = system("docker compose up -d pihole");
  return result;
}

string retrieveIP(string& cmd)
{
  /*
    Use the command that is commented out to send. Then use C-style to send the command, grab the string and output it
     ---> 
  */
  //return system("docker exec pihole hostname -i");
}

string requestsTest(string& pihole_ip, string& url)
{ 
  // TODO: need to finish the check from result
  // Idea is it will use dig to send a request and depending on a response we send a message bacl
  string cmd = "dig @" + pihole_ip + " " + url;
  string result = retrieveIP(cmd);
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

  // once gauging the ip for pihole
  string pihole_ip = retrieveIP();

  vector<string> good_urls= <"google.com", "github.com">;
  vector<string> bad_urls= <"aws-update.net","12724.xyz">;

  //Good tests
  for(string urls : bad_urls)
    {
       // TODO: need to finish the check from result
      string result = requestsTest(pihole_ip,url);

      if(
      
    }

  //Bad tests
  for(string url : bad_urls)
  {
    // TODO: need to finish the check from result
    string result = requestsTest(pihole_ip, url);

    if(result == "0.0.0.0")
    {

    }
  }

  return 0;
}
