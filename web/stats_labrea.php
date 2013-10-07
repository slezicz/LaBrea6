<?
$starttime=time();
require_once("db.inc.php");
require_once("build_table.php");
require_once("build_table_html.php");

$colTrendRise="red";
$colTrendFall="green";
$colPortWell="blue";
$colPortReg="darkorange";
$colPortDyn="black";

$debug=false;
$table="tarpit";

?>

<table width=100%>
<tr valign=top>

<td>

<div><img src="graph_labrea_attackers_count.php"></div>

</td>
<td valign=top>


<div><h2>Host search</h2>
  <form action="stats_host.php">
    <label for="ip">Input IP address</label>
    <input name="ip" type="text" />
    <input type="submit"/>
  </form>
</div>


<?
print ("<div>\n<h2>Overall</h2>");
$q = "select from_unixtime(min(time)) as mi, from_unixtime(max(time)) as ma from ".$table;
if (($debug)) print("<div>DEBUG: $q</div>\n");
$res = mysql_query($q, $link);
$tmp = mysql_fetch_assoc($res);
print("Time frame: ".$tmp['mi']." - ".$tmp['ma']."<br>\n");

/*
* We don't suppot banwidth function yet..
*/
/*
$q = "select from_unixtime(time) as t,bw from ".$table." order by time desc limit 1";
if ($debug) print("<div>DEBUG: $q</div>\n");
$res = mysql_query($q, $link);
$tmp = mysql_fetch_assoc($res);
print("Last bw at: ".$tmp['t']." - <b>".$tmp['bw']."b/s</b><br>");
*/

$q = "select from_unixtime(time) as t, time, ipsrc as ips from ".$table." order by time desc limit 1;";
#$q = "select max(time) as t from tarpit";
if ($debug) print("<div>DEBUG: $q</div>\n");
$res = mysql_query($q, $link);
$tmp = mysql_fetch_assoc($res);
print("Last tarpit at: ".$tmp['t']." (<b>".round((time()-$tmp['time'])/60)."m ago</b>) from <a href=\"stats_host.php?ip=".$tmp['ips']."\">".$tmp['ips']."</a> ( ".gethostbyaddr($tmp['ips'])." )<br>");
#print("Last tarpit at: ".strftime("%F %T", $tmp['t'])." (<b>".round((time()-$tmp['t'])/60)."m ago</b>)<br>");

print("Legend: ");
print("<span style='color:$colTrendRise;'>Port rise</span> ");
print("<span style='color:$colTrendFall;'>Port fall</span> ");
print(" &nbsp; &nbsp; ");
print("<span style='color:$colPortWell;'>Well-known port</span> ");
print("<span style='color:$colPortReg;'>Registered port</span> ");
print("<span style='color:$colPortDyn;'>Dynamic/private/local port</span>");

print("</div>\n");




print ("<div>\n<h2>Port trends</h2>");
print ("trend = count(dpst) - avg(last 72 hours)");
$last = time()-3600*24;

# dnesni pocty
$q = "select pdst, count(pdst) as count from tarpit where time>$last group by pdst order by count desc limit 20;";
if ($debug) print("<div>DEBUG: $q</div>\n");
$res = mysql_query($q, $link);
$data =array();
while ($tmp = mysql_fetch_assoc($res)) { 
	#array_push($data, $tmp);
	$data[$tmp['pdst']] = $tmp['count'];
}

# prumer za posledni 3 dny
$last72 = $last - 3600*24*3;
# /3 je protoze chci dostat prumer na den za posledni 3 dny
$q = "select pdst, round(count(pdst)/3) as count from tarpit where time>$last72 and time<$last group by pdst order by count desc limit 20;";
if ($debug) print("<div>DEBUG: $q</div>\n");
$res = mysql_query($q, $link);
$data72=array();
while ($tmp = mysql_fetch_assoc($res)) { 
	$data72[$tmp['pdst']] = $tmp['count'];
}
#print("<pre>");
#var_dump($data);
#var_dump($data72);
#print("</pre>");

# trend = dnesni / prumer_za_posledni_3
# musim vzit vsechna data z dneska a pripocitat k tomu cely vektor 72, tak dostanu pole ktere ma unikatni klice
# z obou poli a nemusim se starat o dopocitavani zbytku klicu z jednoho nebo druheho pole kdybych to delal jenom na jeden pruchod podle jednoho z poli
# jiste existuje jeste elegantnejsi reseni
$dataTrend=$data;
$dataFinal=array();
foreach (array_keys($data72) as $tmp) {
	#$dataTrend[$tmp] = $dataTrend[$tmp] - $data72[$tmp];
	$dataTrend[$tmp] = round(($data[$tmp] ? $data[$tmp] : 1) / ($data72[$tmp] ? $data72[$tmp] : 1), 3);
}

# vysledky trendu - tabulka
print("<table border=1 cellspacing=0 cellpadding=2>\n");
#$fmt="<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>";
$fmt="<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>\n";
printf($fmt, "pdst/name", "count", "count72", "trendRatio");
foreach (array_keys($dataTrend) as $tmp) {
	$trendcol = ($dataTrend[$tmp] > 1 ? $colTrendRise : $colTrendFall);
	if($tmp < 1024) {
		$portcol=$colPortWell; 
	} elseif ($tmp < 49152) {
		$portcol=$colPortReg;
	} else {
		$portcol=$colPortDyn;
	}

	printf($fmt, 
		"<span style='color:$portcol;'>".$tmp."/".getservbyport($tmp,"tcp")."</span>",
		"<span style='color:grey;'>".( $data[$tmp] ? $data[$tmp] : 0)."</span>", 
		"<span style='color:grey;'>".( $data72[$tmp] ? $data72[$tmp] : 0)."</span>", 
		"<span style='color:$trendcol;'>".$dataTrend[$tmp]."</span>" 
	);
	#var_dump($tmp);
}
print("</table>\n");
print("</div>\n");







print ("<div><h2>Top 10 attackers for last 72 hours</h2>");
$last = time()-3600*72;
$q = "select ipsrc as ips,count(ipsrc) as count, from_unixtime(min(time)) as first, from_unixtime(max(time)) as last from tarpit where time > $last group by ipsrc order by count desc limit 10";
if ($debug) print("<div>DEBUG: $q</div>\n");
$res = mysql_query($q,$link);
$data =array();
while ($tmp = mysql_fetch_assoc($res)) {
	$tmp['hostname']= gethostbyaddr($tmp['ips']);
	$tmp['ips'] = "<a href=\"stats_host.php?ip=".$tmp['ips']."\">".$tmp['ips']."</a>";
	array_push($data, $tmp);
}
if(empty($data))
    print("No attacker in last 72 hours.");
else 
  print ("<pre>".build_table_html($data)."</pre>\n");
#var_dump($data);
print("</div>");


print("<div>\n");
print("<h2>Ownnet attackers for 30 days</h2>\n");
$where = ownnetwhere();

$last7 = time() - 3600*24*30;
if ($debug) print("<div>DEBUG: last7:$last7 - ".strftime("%F %T", $last7)."</div>\n");
$q = "select from_unixtime(min(time)) as time, round((max(time)-min(time))/60) as held, ipsrc as ips,ipdst as ipd,psrc,pdst, count(ipsrc) as count from ".$table." where time>$last7 and ($where) group by ipsrc order by time desc";
#print($q);
if ($debug) print("<div>DEBUG: $q</div>\n");
$res = mysql_query($q,$link);
$data =array();
while ($tmp = mysql_fetch_assoc($res)) {
	$tmp['hostname']= gethostbyaddr($tmp['ips']);	
	$tmp['ipd'] = preg_replace("/^2001:718:1801:/", "", $tmp['ipd']);
	$tmp['ips'] = "<a href=\"stats_host.php?ip=".$tmp['ips']."\">".$tmp['ips']."</a>";
	array_push($data, $tmp);
}
#var_dump($data);
if(count($data)) {
	print(build_table_html($data));
} else {
	print("none?");
}
print("</div>");


?>
</td></tr>
</table>

<div>Generated in <? print time()-$starttime; ?> secs at <? print strftime("%F %T") ?>.</div>
