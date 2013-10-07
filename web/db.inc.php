<?
$host = "localhost"; 
$db = "labrea";
$user = "labrea"; 
$pass = "Labrea6*";

$link = mysql_pconnect($host, $user, $pass);
if (!$link) { die('Could not connect: ' . mysql_error()); }
mysql_select_db($db);

function ownnetwhere() {
	#nacist seznam ownnet
	$filename = "ownnet.rangesint";
	$ownnet = file_get_contents($filename);
	$ownnet = split("[ \n]", trim($ownnet));
	#var_dump($ownnet);
	# where pro sql
	$where = '';
	for($i=0; $i<count($ownnet); $i=$i+2) {
		$length = strlen($ownnet[$i]);
		$where .= "SUBSTRING(ipdst,1,".$length.") like '".$ownnet[$i]."' OR ";
	}
	#posledni \n
	$where=preg_replace("/\ OR\ $/", "", $where);
	#var_dump($where);

	return $where;
}


?>
