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

string piholeIP()
{
  return system("docker exec pihole hostname -i");
}

int requestsTest(string url)
{ 
  // TODO: need to finish the check from result
  // Idea is it will use dig to send a request and depeding on a response we send a message bacl
  string result = system();
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
  string pihole_ip = piholeIP();

  vector<string> good_urls= <"google.com", "github.com">;
  vector<string> bad_urls= <>;

  //Good tests
  for(string urls : bad_urls)
    {
       // TODO: need to finish the check from result
      requestsTest(url);
    }

  //Bad tests

  for(string url : bad_urls)
  {
    // TODO: need to finish the check from result
    string result = requestsTest(url);

    if(result !=)
    {

    }
  }

  return 0;
}
