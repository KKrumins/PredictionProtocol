//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package predictionrouting.simulations;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.inet.StandardHost;
import inet.node.inet.SensorNode;
import inet.physicallayer.apskradio.packetlevel.ApskScalarRadioMedium;
import inet.physicallayer.energyconsumer.SensorStateBasedCcEnergyConsumer;
import inet.power.contract.IEnergyStorage;
import inet.power.generator.AlternatingEpEnergyGenerator;
import inet.power.management.SimpleEpEnergyManagement;
import inet.power.storage.SimpleEpEnergyStorage;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.visualizer.integrated.IntegratedMultiVisualizer;
import inet.visualizer.integrated.IntegratedVisualizer;
import inet.node.aodv.AodvRouter;
//
// TODO auto-generated type
//

simple clientNode
{
    parameters:
        @display("i=misc/sensor2;bgb=446,196");

        @signal[sentPacketCount];
        @statistic[sentPacketCount](title="Number of packets sent"; record=vector,last; interpolationmode=linear; );
        @signal[energyCapacity];
        @statistic[energyCapacity](title="Energy storage"; record=vector,last; interpolationmode=linear; );
        @signal[hopCount];
        @statistic[hopCount](title="HopCount"; record=vector,last; interpolationmode=linear; );
        @signal[lostPackets];
        @statistic[lostPackets](title="LostPckCount"; record=vector,last; interpolationmode=linear; );

        @signal[Node1_Node2];
        @statistic[Node1_Node2](title="Node1 to Node2 link quality"; record=vector,last; interpolationmode=linear; );
        @signal[Node2_Node1];
        @statistic[Node2_Node1](title="Node2 to Node1 link quality"; record=vector,last; interpolationmode=linear; );

        @signal[PDR];
        @statistic[PDR](title="Packet delivery ratio"; record=vector,last; interpolationmode=linear; );

        @signal[SuccessRate];
        @statistic[SuccessRate](title="Rout sent cnt"; record=vector,last; interpolationmode=linear; );

        @signal[Latency];
        @statistic[Latency](title="Avg latency"; record=vector,last; interpolationmode=linear; );

        @signal[TotalPacketCount];
        @statistic[TotalPacketCount](title="Total_packet_count"; record=vector,last; interpolationmode=linear; );

        int myId;
        bool useBestRoutes = true;
        bool sabotageRoutes = true;
        double capacity;
    gates:
        input radioIn @directIn;
}

simple server
{
    @display("i=misc/sensor2");
    gates:
        input radioIn @directIn;
}

network Network
{
    parameters:
        @display("bgb=887,599");
    submodules:
        visualizer: IntegratedVisualizer {
            parameters:
                @display("p=47.034,113.364");
        }
        radioMedium: ApskScalarRadioMedium {
            @display("p=47.38,32.96");
        }
        clientNode1: clientNode {
            @display("p=163,330");
        }
        clientNode2: clientNode {
            @display("p=182,128");
        }
        clientNode3: clientNode {
            @display("p=348,59");
        }
        clientNode4: clientNode {
            @display("p=283,434");
        }
        clientNode8: clientNode {
            @display("p=481,148");
        }
        clientNode5: clientNode {
            @display("p=464,369");
        }
        clientNode6: clientNode {
            @display("p=578,261");
        }
        clientNode7: clientNode {
            @display("p=708,349");
        }
        clientNode10: clientNode {
            @display("p=835,270");
        }
        clientNode9: clientNode {
            @display("p=717,187");
        }
}

network Network20
{
    parameters:
        @display("bgb=1187.91,713.952");
    submodules:
        visualizer: IntegratedVisualizer {
            parameters:
                @display("p=47.034,113.364");
        }
        radioMedium: ApskScalarRadioMedium {
            @display("p=47.38,32.96");
        }
        clientNode1: clientNode {
            @display("p=163,330");
        }
        clientNode2: clientNode {
            @display("p=182,128");
        }
        clientNode3: clientNode {
            @display("p=348,59");
        }
        clientNode4: clientNode {
            @display("p=283,434");
        }
        clientNode8: clientNode {
            @display("p=481,148");
        }
        clientNode5: clientNode {
            @display("p=464,369");
        }
        clientNode6: clientNode {
            @display("p=578,261");
        }
        clientNode7: clientNode {
            @display("p=708,349");
        }
        clientNode10: clientNode {
            @display("p=835,270");
        }
        clientNode9: clientNode {
            @display("p=717,187");
        }
        clientNode20: clientNode {
            @display("p=326.826,247.23");
        }
        clientNode11: clientNode {
            @display("p=143.514,561.996");
        }
        clientNode12: clientNode {
            @display("p=481.194,546.318");
        }
        clientNode13: clientNode {
            @display("p=687.42,531.846");
        }
        clientNode14: clientNode {
            @display("p=868.32,453.456");
        }
        clientNode15: clientNode {
            @display("p=611.442,79.596");
        }
        clientNode16: clientNode {
            @display("p=879.174,77.184");
        }
        clientNode17: clientNode {
            @display("p=1068.516,142.308");
        }
        clientNode18: clientNode {
            @display("p=1056.456,287.028");
        }
        clientNode19: clientNode {
            @display("p=1056.456,414.864");
        }
}
