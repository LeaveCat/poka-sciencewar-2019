<?php
require_once "config.php";
if(isset($_COOKIE['token'])){
	$jwt = jwt_decode($_COOKIE['token']);
	if(!isset($jwt->uid)){
		die("Invalid Token");
	}
}else{
	header('Location: ./');
}


$head = <<<EOD
<html>
<head></head>
<body>
<center><h1>FLAG Shop 2019</h1></center>
<br/><hr/>
<table border=1>
	<tr>
		<td>Product</td>
		<td>money</td>
		<td>Comment</td>
	</tr>
EOD;

$tail = <<<EOD
</body>
</html>
EOD;

if($jwt->buy){
	$fn = tempnam ('/tmp', 'shop_tmp');
	$html_fn = $fn . ".html"; 
	$pdf_fn  = $fn . ".pdf"; 
	if ($html_fn){
		$f = fopen ($html_fn, 'w+');
		if ($f){
			$template = $head;
			$total = 0;
			foreach(array_keys($jwt->buy) as $index) {
				$template .= "<tr>";
				$template .= "<td>". $jwt->buy[$index]->name ."</td>";
				$template .= "<td>". $jwt->buy[$index]->money ."</td>";
				$template .= "<td>". $jwt->buy[$index]->comment ."</td>";
				$template .= "</tr>";
				$total += $jwt->buy[$index]->money;
			}
			$template .= "</table><br/><hr/>";
			$template .= "<p align='right'>Total : ".$total."</p>";
			$template .= $tail;
			fwrite($f, $template);
		
		fclose($f);
		$command = "wkhtmltopdf " . escapeshellcmd($html_fn) . " " . escapeshellcmd($pdf_fn);
		system($command);

		if(file_exists($pdf_fn)){
			header("Content-type:application/pdf");
			header('Content-Disposition: attachment; filename=download.pdf');
			//No cache
			header('Expires: 0');
			header('Cache-Control: must-revalidate');
			header('Pragma: public');

			//Define file size
			header('Content-Length: ' . filesize($pdf_fn));

			ob_clean();
			flush();
			readfile($pdf_fn);
			unlink($html_fn);
			unlink($pdf_fn);
			unlink($fn);
			exit;
		}

		}
	}
}

?>