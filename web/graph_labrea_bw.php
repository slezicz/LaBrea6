<?php
include ("jpgraph/src/jpgraph.php");
include ("jpgraph/src/jpgraph_log.php");
include ("jpgraph/src/jpgraph_line.php");
require_once("jpgraph/src/jpgraph_date.php"); 

require_once("db.inc.php");

#date_default_timezone_set('UCT');
$debug = false;

$table = "tarpit";



switch($_GET['type']) {
	case 'week':
		//last week
		$last = time() - (3600*24)*7;
		// agreg by 10 minutes
		$aggregate = 60*10;
		// day = 3600 sec * 24 = 86400
		$title = "LaBrea week bandwidth usage";
		break;
	case 'month':
		//min*hour*day*month
		$last = time() - (3600*24)*30;
		// agreg by hours
		$aggregate = 3600;
		$title = "LaBrea month bandwidth usage";
		break;
	case 'year':
		// last year
		$last = time() - (3600*24)*365;
		// agreg by day
		$aggregate = 3600*24;
		$title = "LaBrea year bandwidth usage";
		break;
}

$q = "select (time-(time%$aggregate)) as t, count(distinct ipsrc) b from ".$table." where time > $last group by t";
$res = mysql_query($q, $link);
if(!$res) { print("ERROR: err in query".mysql_error()); }
$ydata = array();
$datax = array();
while ($tmp = mysql_fetch_array($res)) {
	array_push($datax, $tmp[0]);
	array_push($ydata, $tmp[1]);
}
if($debug) {
	print("AAAAAAAAAAAA: ".strftime("%F %T", time()));
	var_dump($datax);
	foreach ( $datax as $tmp ) print("<div>".strftime("%F %T", $tmp)."</div>");
	var_dump($ydata);
}

$graph = new Graph(400,300,"auto", 10);
# SetMargin(left,right, top, bot) 
$graph->img->SetMargin(50,20,40,130);
#$graph->SetScale("textlog");
$graph->SetScale("datlog");
#$graph->SetScale("datlin");
$graph->title->Set($title);
#$graph->title->SetFont(FF_FONT1,FS_BOLD);

$graph->xgrid->Show(true,false);
$graph->ygrid->Show(true,true);

#$graph->xaxis->title->Set("Date");
$graph->xaxis->SetTitle("Date");
$graph->xaxis->SetTitleSide(SIDE_TOP); 
$graph->xaxis->SetTextTickInterval(100);
$graph->xaxis->scale-> SetDateFormat( 'Y-m-d H:i');
$graph->xaxis->SetLabelAngle(90);
#$graph ->xaxis->scale-> SetDateAlign( DAYADJ_1);
#$graph->xaxis->SetTickLabels($datax);

$graph->yaxis->SetTitle("Bw [B/s]");
$graph->xaxis->SetTitleSide(SIDE_TOP); 
$graph->yaxis->scale->ticks->SupressFirst();

$lineplot=new LinePlot($ydata, $datax);
$graph->Add($lineplot);
$lineplot->SetColor("blue");
$lineplot->SetWeight(2);

#$lineplot->value->SetColor("darkred");
#$lineplot->value->SetFont( FF_FONT1, FS_BOLD);
#$lineplot->value->SetFormat( "%d");
#$lineplot->value->Show();

// Display the graph
$graph->Stroke();

?>
