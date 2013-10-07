#! /usr/bin/env python
import sys
from scapy import *

for 1 in 10 :
  send(IPv6(src="2001:718:1801:1238:216:3eff:fe23:"+hex(random.randint(1, 65535))[2:], dst="2001:718:1801:1238:216:3eff:fe23:"+hex(random.randint(1, 65535))[2:])/TCP(sport=RandShort(), dport=RandShort()%1024, flags="S"))
  send(IPv6(src="2001:718:1801:1238:216:3eff:fe23:"+hex(random.randint(1, 65535))[2:], dst="2001:718:1801:1238:216:3eff:fe23:"+hex(random.randint(1, 65535))[2:])/TCP(sport=RandShort(), dport=[22,80,8080,443,137,138,139], flags="S"))
