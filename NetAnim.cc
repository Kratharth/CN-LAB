#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nCsma = 3;

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;

  NodeContainer p2pNodes1;
  p2pNodes1.Create (2);

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes1.Get (1));
  csmaNodes.Create (nCsma);
  

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices1;
  p2pDevices1 = pointToPoint.Install (p2pNodes1);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (p2pNodes1.Get (0));
  stack.Install (csmaNodes);
  
  Ipv4AddressHelper address1,address2;
  address1.SetBase ("10.1.1.0", "255.255.255.0");
  address2.SetBase("10.3.3.0","255.255.255.0");
  
  Ipv4InterfaceContainer p2pInterfaces1;
  p2pInterfaces1= address1.Assign (p2pDevices1);

  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address2.Assign (csmaDevices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (2));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient1 (csmaInterfaces.GetAddress (2), 9);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps1 = echoClient1.Install (p2pNodes1.Get (0));
  clientApps1.Start (Seconds (2.0));
  clientApps1.Stop (Seconds (10.0));
  
 

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
 
 AnimationInterface anim("anim.xml");
  anim.SetConstantPosition(p2pNodes1.Get(0),1.0,2.0);
  anim.SetConstantPosition(p2pNodes1.Get(1),30.0,40.0);
  anim.SetConstantPosition(csmaNodes.Get(1),90.0,100.0);
  anim.SetConstantPosition(csmaNodes.Get(2),120.0,120.0);  
  anim.SetConstantPosition(csmaNodes.Get(3),130.0,150.0);
  anim.EnablePacketMetadata(true);

  

  
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
