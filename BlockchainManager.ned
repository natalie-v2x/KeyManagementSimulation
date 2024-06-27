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

package lte.stack.bcManager;

import lte.x2.X2User;


simple BlockchainManager like X2User
{
    parameters:
        @signal[txDelay](type="simtime_t");
        @statistic[TransactionDelay](title="transaction delay"; source="txDelay"; record=vector);
        @signal[bkDelay](type="simtime_t");
        @statistic[BlockDelay](title="block delay";source="bkDelay";record=vector, mean);
        @signal[cDelay](type="simtime_t");
        @statistic[ConsensusDelay](title="consensus delay";source="cDelay";record=vector, mean);
        @display("i=block/cogwheel");
        @class("BlockchainManager");
        
    gates:
        //# connections to the X2 Manager
        input x2ManagerIn;
        output x2ManagerOut;
}
