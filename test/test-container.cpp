

using namespace std;

void piholeTest()
{
    /* 
        TODO: -> Setup connection to the individual container
              -> Setup request to send
              -> Grab sites 
              -> Use dig to request those sites
              -> Add tests to check OK/NOK for connection
              -> Return 
    */
}

void wireguardTest()
{
    /*
        TODO: -> Setup commection to the individual docker container
              -> Setup the Peer.conf
              -> Setup VPN Tunnel connection between Wireguard and a fake device
              -> Gauge connection to Wireguard
              -> Return  
    */
}

void 

int main ()
{
    try
        {
            piholeTest();
        }
    catch(const exception& e)
        {
            cout << "There is an error: " + e << endl;
        }
    catch
        {
            cout << "There is an unexpected error"<<endl;
        }

      try
        {
            wireguardTest();
        }
    catch(const exception& e)
        {
            cout << "There is an error: " + e << endl;
        }
    catch
        {
            cout << "There is an unexpected error"<<endl;
        }

}
