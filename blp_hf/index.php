<?php
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {

        // Read POST parameters
        $name  = $_POST['name']  ?? '';
        $score = $_POST['score'] ?? '';

        // Simple validation (optional)
        if ($name !== '' && $score !== '') {

            // Path to CSV file
            $file = 'scores.csv';

            // Open the file in append mode
            $handle = fopen($file, 'a');

            // Write a new line: name,score
            fputcsv($handle, [$name, $score]);

            fclose($handle);
        }
    }

    $data = [];
    if (($csvFile = fopen("scores.csv", "r")) !== false) {
        while (($row = fgetcsv($csvFile)) !== false) {
            $data[] = $row;
        }
        fclose($csvFile);

        $first_row = array_shift($data);
        $scores = array_column($data,1);
        array_multisort($scores, SORT_DESC, $data);
        array_unshift($data, $first_row);
    }
    else {
        // handle error (file not found / permission)
        $data = [];
    }
?>

<!DOCTYPE html>
<html lang="hu">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="stylesheet" href="./style.css">
    </head>
    <body>
        <table>
        <?php if (!empty($data)): ?>
            <?php
            // Use first row as header
            $header = array_shift($data);
            ?>
            <thead>
                <tr>
                    <?php foreach ($header as $cell): ?>
                        <th><?= htmlspecialchars($cell, ENT_QUOTES | ENT_SUBSTITUTE, 'UTF-8') ?></th>
                    <?php endforeach; ?>
                </tr>
            </thead>
            <tbody>
                <?php foreach ($data as $row): ?>
                    <tr>
                        <?php foreach ($row as $cell): ?>
                            <td><?= htmlspecialchars($cell, ENT_QUOTES | ENT_SUBSTITUTE, 'UTF-8') ?></td>
                        <?php endforeach; ?>
                    </tr>
                <?php endforeach; ?>
            </tbody>
        <?php else: ?>
            <tr><td>Nincs megjeleníthető adat.</td></tr>
        <?php endif; ?>
    </table>
    </body>
</html>