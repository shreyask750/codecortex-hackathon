const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');
const path = require('path');

const app = express();
const port = 3000;

const cors = require('cors');
app.use(cors());

// Middleware to parse incoming form data
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// Serve static files (like your index.html)
app.use(express.static('public'));


app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'website.html'));
});

// Handle form submissions and run Python script
app.post('/predict', (req, res) => {
    // Extract the form data from the request body
    const {
        flow,
        chilledWaterSupplyTemperature,
        chilledWaterReturnTemperature,
        plantEfficiency,
        ambientTemperature,
        totalChillerPower,
        relativeHumidity,
        wetBulbTemperature,
    } = req.body;

    // Construct the command to run your Python script with these inputs
    const command = `python app.py ${ambientTemperature} ${relativeHumidity} ${wetBulbTemperature} ${chilledWaterSupplyTemperature} ${chilledWaterReturnTemperature} ${plantEfficiency} ${totalChillerPower} ${flow}`;

    // Execute the Python script with the form data as arguments
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error executing Python script: ${error}`);
            return res.status(500).send('Internal Server Error');
        }

        // Render the output.html page with the prediction result
        res.render('output', { result: stdout.trim() });
    });
});

// Set the view engine to render HTML files
app.set('views', path.join(__dirname, 'public'));
app.set('view engine', 'ejs');

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
