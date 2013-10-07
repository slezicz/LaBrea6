<?php
include ("jpgraph/src/jpgraph.php");
include ("jpgraph/src/jpgraph_log.php");
include ("jpgraph/src/jpgraph_line.php");
require_once("jpgraph/src/jpgraph_date.php"); 

include ("jpgraph/src//jpgraph_scatter.php");

require_once("db.inc.php");

$table = "tarpit";



$aggregate = 3600*24;
$last = 3600*24*365;
$title = "LaBrea attackers per day";

$q = "select (time-(time%$aggregate)) as t, count(distinct ipsrc) from ".$table." where time>$last group by t;";
$res = mysql_query($q, $link);
if(!$res) { print("ERROR: err in query".mysql_error()); }
$ydata = array();
$datax = array();
while ($tmp = mysql_fetch_array($res)) {
	array_push($datax, $tmp[0]);
	array_push($ydata, $tmp[1]);
}
#var_dump($datax);
#var_dump($ydata);

$graph = new Graph(400,300,"auto", 10);
# SetMargin(left,right, top, bot) 
$graph->img->SetMargin(50,20,40,130);
#$graph->SetScale("textlog");
#$graph->SetScale("datlog");
$graph->SetScale("datlin");
$graph->title->Set($title);
#$graph->title->SetFont(FF_FONT1,FS_BOLD);

$graph->xgrid->Show(true,false);
$graph->ygrid->Show(true,true);

#$graph->xaxis->title->Set("Date");
$graph->xaxis->SetTitle("Date");
$graph->xaxis->SetTitleSide(SIDE_TOP); 
$graph->xaxis->SetTextTickInterval(100);
$graph->xaxis->scale->SetDateFormat( 'Y-m-d H:i');
$graph->xaxis->SetLabelAngle(90);
#$graph ->xaxis->scale-> SetDateAlign( DAYADJ_1);
#$graph->xaxis->SetTickLabels($datax);

$graph->yaxis->SetTitle("Number of attackers");
$graph->xaxis->SetTitleSide(SIDE_TOP); 
$graph->yaxis->scale->ticks->SupressFirst();

#$lineplot=new ScatterPlot($ydata, $datax);
#$lineplot->mark->SetType(MARK_CROSS);


$lineplot=new LinePlot($ydata, $datax);



$graph->Add($lineplot);
$lineplot->SetColor("red");
$lineplot->SetWeight(1);

#$lineplot->value->SetColor("darkred");
#$lineplot->value->SetFont( FF_FONT1, FS_BOLD);
#$lineplot->value->SetFormat( "%d");
#$lineplot->value->Show();

// Display the graph
$graph->Stroke();

?>
