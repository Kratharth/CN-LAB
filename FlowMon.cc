 #include "ns3/core-module.h"
 #include "ns3/network-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/point-to-point-module.h"
 #include "ns3/applications-module.h"
 #include "ns3/flow-monitor-module.h"
 
 using namespace ns3;
 
 NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");
 
 int
 main (int argc, char *argv[])
 {
   CommandLine cmd;
   cmd.Parse (argc, argv);
   
   Time::SetResolution (Time::NS);
   LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
   LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
 
   NodeContainer nodes1;
   nodes1.Create (2);
 
   PointToPointHelper pointToPoint1;
   pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
   pointToPoint1.SetChannelAttribute ("Delay", StringValue ("2ms"));
 
   NetDeviceContainer devices1;
   devices1 = pointToPoint1.Install (nodes1);
 
   InternetStackHelper stack;
   stack.Install (nodes1);
 
   Ipv4AddressHelper address1;
   address1.SetBase ("10.2.2.0", "255.255.255.0");
 
   Ipv4InterfaceContainer interfaces1;
   interfaces1 = address1.Assign (devices1);
 
   UdpEchoServerHelper echoServer (9);
 
   ApplicationContainer serverApps = echoServer.Install (nodes1.Get (1));
   serverApps.Start (Seconds (1.0));
   serverApps.Stop (Seconds (505.0));
 
   UdpEchoClientHelper echoClient(interfaces1.GetAddress(1), 9);
   echoClient.SetAttribute ("MaxPackets", UintegerValue (500));
   echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
 
   ApplicationContainer clientApps = echoClient.Install (nodes1.Get(0));
   clientApps.Start (Seconds (2.0));
   clientApps.Stop (Seconds (503.0));


    std::cout<<std::endl;
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();
    NS_LOG_INFO ("Run Simulation.");
    Simulator::Stop (Seconds(510.0));
    Simulator::Run ();
    monitor->CheckForLostPackets ();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier ( ));
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      std::cout<<" t.sourceAddress = "<<t.sourceAddress<<", t.destinationAddress = "<<t.destinationAddress<<" Source port:"<<t.sourcePort<<" "<<"Destination port:"<<t.destinationPort<<"\n";

        std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " <<t.destinationAddress << ")\n";
        std::cout << " Tx Bytes: " << i->second.txBytes << "\n";
        std::cout << " Rx Bytes: " << i->second.rxBytes << "\n";
        std::cout << " Tx Packets: " << i->second.txPackets << "\n";
        std::cout << " Rx Packets: " << i->second.rxPackets << "\n";
        std::cout << " Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1000 / 1000 << "Mbps\n";
    }
    monitor->SerializeToXmlFile("flowmon.flowmon", true, true);
    Simulator::Destroy ();
    NS_LOG_INFO ("Done.");

    return 0;
 }
