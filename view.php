<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="refresh" content="5">
    <title>IoT Data</title>
    <style>
        /* Your existing styles here */

        /* Dropdown styling */
        .dropdown {
            position: relative;
            display: inline-block;
        }

        .dropdown button {
            background-color: #4CAF50;
            color: white;
            padding: 12px 16px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }

        .dropdown-content {
            display: none;
            position: absolute;
            background-color: #f9f9f9;
            min-width: 160px;
            box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2);
            z-index: 2;
            border-radius: 4px;
        }

        .dropdown:hover .dropdown-content {
            display: block;
        }

        .dropdown-content a {
            padding: 12px 16px;
            display: block;
            text-decoration: none;
            color: #333;
        }

        .dropdown-content a:hover {
            background-color: #ddd;
        }

        /* Table styling */
        #c4ytable {
            font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
            border-collapse: collapse;
            width: 100%;
            margin-top: 20px;
        }

        #c4ytable th, #c4ytable td {
            border: 1px solid #ddd;
            padding: 8px;
        }

        #c4ytable th {
            padding-top: 12px;
            padding-bottom: 12px;
            text-align: left;
            background-color: #00A8A9;
            color: white;
        }

        #c4ytable tr:nth-child(even) {
            background-color: #f2f2f2;
        }

        #c4ytable tr:hover {
            background-color: #ddd;
        }
    </style>
</head>
<body>
    <div id="cards" class="cards">

        <!-- Dropdown Menu -->
        <div class="dropdown">
            <button>
                <?php
                    $selectedSensorId = isset($_GET['sensor_id']) ? $_GET['sensor_id'] : null;
                    echo $selectedSensorId ? "Sensor ID: " . $selectedSensorId : "Choose Sensor ID";
                ?>
            </button>
            <div class="dropdown-content">
                <?php
                    $servername = "localhost";
                    $username = "root";
                    $password = " ";
                    $dbname = " ";

                    $conn = new mysqli($servername, $username, $password, $dbname);

                    if ($conn->connect_error) {
                        die("Database Connection failed: " . $conn->connect_error);
                        echo "<a href='install.php'>If first time running click here to install database</a>";
                    }

                    $sql = "SELECT id FROM sensor";
                    $result = mysqli_query($conn, $sql);

                    while ($row = mysqli_fetch_assoc($result)) {
                        echo "<a href='?sensor_id=" . $row['id'] . "'>" . $row['id'] . "</a>";
                    }

                    mysqli_close($conn);
                ?>
            </div>
        </div>

        <?php 
            $selectedSensorId = isset($_GET['sensor_id']) ? $_GET['sensor_id'] : null;

            $conn = new mysqli($servername, $username, $password, $dbname);

            if ($conn->connect_error) {
                die("Database Connection failed: " . $conn->connect_error);
            }

            $sql = "SELECT sensor.location, thongso.*
                    FROM thongso
                    INNER JOIN sensor ON thongso.sensor_id = sensor.id";

            if ($selectedSensorId) {
                $sql .= " WHERE sensor.id = " . $selectedSensorId;
            }

            $sql .= " ORDER BY sensor.id";

            if ($result = mysqli_query($conn, $sql)) {
                echo "<TABLE id='c4ytable'>";
                echo "<TR><TH>ID</TH><TH>Location</TH><TH>Nhiệt Độ (C)</TH><TH>Độ Ẩm</TH><TH>Độ Ẩm Đất (%)</TH><TH>Date</TH><TH>Time</TH></TR>";

                while ($row = mysqli_fetch_row($result)) {
                    //echo "<TR>";
                    echo "<TD>".$row[1]."</TD>"; // Location from sensor table
                    echo "<TD>".$row[0]."</TD>"; // Thongso.id
                    echo "<TD>".$row[2]."</TD>"; // Thongso.nhietdo
                    echo "<TD>".$row[3]."</TD>"; // Thongso.doam
                    echo "<TD>".$row[4]."</TD>"; // Thongso.doamdat
                    echo "<TD>".$row[5]."</TD>"; // Thongso.date
                    echo "<TD>".$row[6]."</TD>"; // Thongso.time
                    echo "</TR>";
                }

                echo "</TABLE>";
                mysqli_free_result($result);
            }

            mysqli_close($conn);
        ?>
    </div>
</body>
</html>