<head>
<style>
#menu li,h2 { display:inline; }
</style>
</head>
<body>
<div id="demo_automenu">

<?
$starttime=time();
require_once("db.inc.php");
require_once("build_table.php");
require_once("build_table_html.php");
require_once("constants.php");
require_once('nqt/NetworkQueryTool.php');

$debug=false;

$ip="NONE";
if( preg_match('/(\d+).(\d+).(\d+).(\d+)/',  $_GET['ip']) ) {
	$ip = $_GET['ip'];
  $tmp = inet_pton($ip);
  $ip = inet_ntop($tmp);
}
?>

<a name="menu"></a>
<div>
	<ul id="menu">
    <li><a href=<? echo '"http://'.$_global_host.$_global_root_url.'"'?>>Back</a></li>
	</ul>
</div>

<?


print ("<div>\n<a name='overall'><h2>Overall</h2><p>");

print("<div>address: $ip<br>\n");
print("hostname: ". gethostbyaddr($ip)."</div>\n");


print ("<div>\n<a name='tarpit'><h2>Tarpit</h2>");
$q="select from_unixtime(time) as time, ipsrc as ips, psrc, ipdst as ipd, pdst, count(pdst) as count_pdst from tarpit where ipsrc='".$ip."' group by pdst";
if (($debug)) print("<div>DEBUG: $q</div>\n");
$res = mysql_query($q, $link);

$data = array();
while ($tmp = mysql_fetch_assoc($res)) {
	array_push($data, $tmp);
}

if(count($data)) {
	print(build_table_html($data));
} else {
	print("<br/> <b>no record found with IP address ".$ip."</b>");
}
print("</div>");




$nqt = new NetworkQueryTool($ip);
exec("whois ".$ip, $whois);
print("<div><h3>RIR whois</h3>\n<pre>");
foreach($whois as $l){
  print($l."\n");
}
print("<div><h3>Ping</h3>\n<pre>");
exec("ping6 -c 1 ".$ip, $ping);
print("</pre></div>");
foreach($ping as $l){
  print($l."\n");
}

print("</pre></div>");
#print("<div><h3>Open ports</h3><ul>");
#foreach (array('21','22','80','137','443','445','1025', '8080') as $tmp) {
#	print("<li>Port $tmp ".($nqt->doCheckPort($tmp) ? ' is open' : ' is closed</li>'));
#}
#print("</ul></div>");

print("</div>");






?>












<div>Generated in <? print time()-$starttime; ?> secs at <? print strftime("%F %T") ?>.</div>
</div>
</body>
</html>
