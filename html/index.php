<?php
	include('conn.php'); // Open DB

//	/*
	$sql =<<<EOF
      INSERT INTO ACRt (ACPID, fr, op, ty) VALUES ('1', '1f', '1', '1');
EOF;

	$sql =<<<EOF
   SELECT ACPID FROM ACRt DESC LIMIT 10;
EOF;

	$sql =<<<EOF
   SELECT ACPID FROM ACRt ORDER BY ACPID DESC LIMIT 1;
EOF;
    $sql =<<<EOF
      SELECT ID, resource.ACPID, ResID, ACRID, fr, op, ty FROM resource INNER JOIN ACRt ON resource.ACPID = ACRt.ACPID;
EOF;

    $sql =<<<EOF
      SELECT * FROM resource INNER JOIN ACRt ON resource.ACPID = ACRt.ACPID;
EOF;

   $ret = $db->query($sql);
	if(!$ret)
      echo "<br /><hr /> &nbsp " . $db->lastErrorMsg();
$row = $ret->fetchArray(SQLITE3_ASSOC);
//print_r($row);
//echo "acpid = " . $row[ACPID];
//   $db->close();
//	*/
?>


<!DOCTYPE html>
<html>
<head>
	<title>OneM2M ACP Management</title>
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.0/jquery.min.js"></script>
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css" />
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
</head>
<body>
<div class="container">
	<div style="height:50px;"></div>
	<div class="well" style="margin:auto; padding:auto; width:80%;">
	<span style="font-size:25px; color:blue"><center><strong>The OneM2M ACP Management Module</strong></center></span>	
		<span class="pull-left"><a href="#addnew" data-toggle="modal" class="btn btn-primary"><span class="glyphicon glyphicon-plus"></span> Add New ACP</a></span>
		<div style="height:50px;"></div>
		<table class="table table-striped table-bordered table-hover">
			<thead>
				<th>Sr.</th>
<!--				<th>ID</th>
				<th>ACPID</th>
				<th>ACRID</th>
-->
				<th>to (ResID)</th>
				<th>fr</th>
				<th>op</th>
				<th>ty</th>
				<th>Actions</th>
			</thead>
			<tbody>
			<?php
			$icounter = 0;
			   while($row = $ret->fetchArray(SQLITE3_ASSOC) ) {
				   $icounter++;
					?>
					<tr>
						<td><?php echo $icounter;   ?></td>
<!--						<td><?php echo $row['ID'];   ?></td>
						<td><?php echo $row['ACPID'];   ?></td>
						<td><?php echo $row['ACRID'];   ?></td>
	-->					<td><?php echo $row['ResID'];   ?></td>
						<td><?php echo $row['fr'];		?></td>
						<td><?php echo $row['op'];		?></td>
						<td><?php echo $row['ty'];		?></td>
						<td>
							<a href="#edit<?php echo $row['ACRID']; ?>" data-toggle="modal" class="btn btn-warning"><span class="glyphicon glyphicon-edit"></span> Edit</a>
							<a href="#del<?php echo $row['ACRID']; ?>" data-toggle="modal" class="btn btn-danger"><span class="glyphicon glyphicon-trash"></span> Delete</a>
							<?php include('button.php'); ?>
						</td>
					</tr>
					
					<?php
				}
			
			?>
			</tbody>
		</table>
	</div>
	<?php	include('add_modal.php'); ?>
</div>
</body>
</html>