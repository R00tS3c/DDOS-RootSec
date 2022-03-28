<?php
 
# CloudFlare Bypasser
# Coded By OmidKiller.
# For Your Projects...
function is_ipv4($ip)
{
    return filter_var($ip, FILTER_VALIDATE_IP, FILTER_FLAG_IPV4) ? $ip : '(Null)';
}
 
$me = $argv[0];
$url = @$argv[1];
 
if(!isset($url))
    die("\n[+] Usage: php {$me} <url>\n[+] Example: php {$me} http://www.omid-killer.info\n");
 
if(!preg_match('/^(https?):\/\/(w{3}|w3)\./i', $url))
    die("\nURL is invalid.\nURL must be formatted as: http(s)://www." . preg_replace('/^(https?):\/\//', '', $url) . "\n(for compatibility reasons)\n");
 
$headers = get_headers($url, 1);
$server = $headers['Server'];
 
$sdom = array('cpanel.', 'ftp.', 'mail.', 'webmail.', 'direct.', 'direct-connect.', 'record.', 'ssl.', 'dns.', 'help.', 'blog.', 'forum.');
$count = count($sdom);
 
if(preg_match('/^(https?):\/\/(w{3}|w3)\./i', $url, $matches))
{
    if($matches[2] != 'www')
    {
    $url = preg_replace('/^(https?):\/\//', '', $url);
    }
    else
    {
    $url = explode($matches[0], $url);
    $url = $url[1];
    }
}
 
if(is_array($server))
    $server = $server[0];
 
if(preg_match('/cloudflare/i', $server))
    echo "\n[+] CloudFlare detected: {$server}\n";
else
    echo "\n[+] CloudFlare wasn't detected, proceeding anyway.\n";
 
echo '[+] IP: ' . is_ipv4(gethostbyname($url)) . "\n\n";
echo "[+] Searching for more IP addresses.\n\n";
 
for($x = 0; $x < $count; $x++)
{
    $site = $sdom[$x] . $url;
    $ip = is_ipv4(gethostbyname($site));
 
    echo "Trying {$site}: {$ip}\n";
}
 
echo "\n[+] Finished.\n";
 
?>


