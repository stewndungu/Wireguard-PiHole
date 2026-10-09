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

void requestsTest()
{
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



  // once gauging the ip then send 
  string pihole_ip = "";

  vector<string> good_urls;
  vector<string> bad_urls;

  for()
    {
      requestsTest();
    }

  return 0;
}
