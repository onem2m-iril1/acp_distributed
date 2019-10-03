<!-- Delete -->
    <div class="modal fade" id="del<?php echo $row['ACRID']; ?>" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                    <center><h4 class="modal-title" id="myModalLabel">Delete</h4></center>
                </div>
                <div class="modal-body">
				<?php
				/*
					$del=mysqli_query($conn,"select * from user where userid='".$row['userid']."'");
					$drow=mysqli_fetch_array($del);
					*/
				?>
				Are you sure.?
				</div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal"><span class="glyphicon glyphicon-remove"></span> Cancel</button>
                    <a href="delete.php?ACRID=<?php echo $row['ACRID'];?>&ACPID=<?php echo $row['ACPID'];?>" class="btn btn-danger"><span class="glyphicon glyphicon-trash"></span> Delete</a>
                </div>
				
            </div>
        </div>
    </div>
<!-- /.modal -->

<!-- Edit -->
    <div class="modal fade" id="edit<?php echo $row['ACRID']; ?>" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                    <center><h4 class="modal-title" id="myModalLabel">Edit</h4></center>
                </div>
                <div class="modal-body">
				<div class="container-fluid">
				<form method="POST" action="edit.php?id=<?php echo $erow['userid']; ?>">
					<div class="row">
						<div class="col-lg-2">
							<label style="position:relative; top:7px;"></label>
						</div>
						<div class="col-lg-10">
							<input type="hidden" name="acrid" class="form-control" value="<?php echo $row['ACRID']; ?>">
						</div>
					</div>
					<div class="row">
						<div class="col-lg-2">
							<label style="position:relative; top:7px;">ResourceID:</label>
						</div>
						<div class="col-lg-10">
							<input type="text" disabled name="resid" class="form-control" value="<?php echo $row['ResID']; ?>">
						</div>
					</div>
					<div style="height:10px;"></div>
					<div class="row">
						<div class="col-lg-2">
							<label style="position:relative; top:7px;">From (fr):</label>
						</div>
						<div class="col-lg-10">
							<input type="text" name="fr" class="form-control" value="<?php echo $row['fr']; ?>">
						</div>
					</div>
					<div style="height:10px;"></div>
					<div class="row">
						<div class="col-lg-2">
							<label style="position:relative; top:7px;">Operation (op):</label>
						</div>
						<div class="col-lg-10">
							<input type="text" name="op" class="form-control" value="<?php echo $row['op']; ?>">
						</div>
					</div>
					<div style="height:10px;"></div>
					<div class="row">
						<div class="col-lg-2">
							<label style="position:relative; top:7px;">Type (ty):</label>
						</div>
						<div class="col-lg-10">
							<input type="text" name="ty" class="form-control" value="<?php echo $row['ty']; ?>">
						</div>
					</div>
					<div style="height:10px;"></div>
					
                </div> 
				</div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal"><span class="glyphicon glyphicon-remove"></span> Cancel</button>
                    <button type="submit" class="btn btn-warning"><span class="glyphicon glyphicon-check"></span> Save</button>
                </div>
				</form>
            </div>
        </div>
    </div>
<!-- /.modal -->