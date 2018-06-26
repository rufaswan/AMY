<?php
if ( $argc < 2 ) die("No file provided\n");

//------------------------------------------------
function bytewrite( $fop, $int, $byte )
{
	$byte1 = ( $int & 0xff000000 ) >> 24;
	$byte2 = ( $int & 0x00ff0000 ) >> 16;
	$byte3 = ( $int & 0x0000ff00 ) >>  8;
	$byte4 = ( $int & 0x000000ff ) >>  0;

	// write in little endian byte order
	if ( $byte >= 1 )  fwrite( $fop, chr($byte4) );
	if ( $byte >= 2 )  fwrite( $fop, chr($byte3) );
	if ( $byte >= 3 )  fwrite( $fop, chr($byte2) );
	if ( $byte >= 4 )  fwrite( $fop, chr($byte1) );
}
//------------------------------------------------
function tmx2map( $layer )
{
	global $map, $fname;

	$layer_name = $layer['@attributes']['name'];
	$layer_w = $layer['@attributes']['width'];
	$layer_h = $layer['@attributes']['height'];
	$layer_data = $layer['data'];

	$data = preg_split("/,/", $layer_data);
	echo "Found number of tiles : " .count($data). "\n";
	echo "Should have be " .$layer_w. " * " .$layer_h. " tiles : " .$layer_w * $layer_h. "\n";
	$written = 0;

	$fop1 = fopen( $fname. '-' .$layer_w. 'x' .$layer_h. '.' .$layer_name. '1', 'wb');
	$fop2 = fopen( $fname. '-' .$layer_w. 'x' .$layer_h. '.' .$layer_name. '2', 'wb');

	//fwrite($fop, chr($layer_w) ); // map width in tiles
	//fwrite($fop, chr($layer_h) ); // map height in tiles

	foreach ( $data as $k => $v )
	{
		$written++;
		$v = trim($v);
		$v = ( $v > 0 ) ? $v - 1 : 0;
		if ( is_numeric($v) )
		{
			bytewrite($fop1, $v, 1 );
			bytewrite($fop2, $v, 2 );
		}
		else
		{
			echo "Error " .$k. "(" .$v. "), replaced with 0\n";
			bytewrite($fop1, 0, 1 );
			bytewrite($fop2, 0, 2 );
		}
		$map[$v] = 1;
	}
	fclose($fop1);
	fclose($fop2);

	$v = $written * 2;
	echo "Written bytes :";
	echo " (map1) " .$written;
	echo " (map2) " .$v;
	echo "\n";
}
//------------------------------------------------

$tmx = $argv[1];
$fname = str_ireplace(".tmx", '', $tmx);
$xml = new SimpleXMLElement( file_get_contents($tmx) );
$json = json_decode( json_encode($xml), 1 );
$map = array();
//print_r($xml);
print_r($json);

if ( isset($json['layer'][0]) )
{
	foreach ( $json['layer'] as $layer )
		tmx2map( $layer );
}
else
	tmx2map( $json['layer'] );

ksort($map);
echo "generating an index file...\n";
$fop = fopen( $fname. '-index.lst', 'wb');
foreach ( $map as $k => $v )
	fwrite($fop, $k."\n");
fclose($fop);
