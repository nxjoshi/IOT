/**************************websocket_example.js*************************************************/

var bodyParser = require("body-parser");
const express = require("express"); //express framework to have a higher level of methods
const app = express(); //assign app variable the express class/method
var http = require("http");
var path = require("path");
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
const server = http.createServer(app); //create a server
const database = require("./database/database.service");
var os = require("os");
const dbConfig = require("./database/database.conf");
const defaultThreadPoolSize = 4;

// Increase thread pool size by poolMax
process.env.UV_THREADPOOL_SIZE =
  dbConfig.cvgData.poolMax + defaultThreadPoolSize;

//**********database startup */

async function startup() {
  try {
    console.log("Initializing database module");

    await database.initialize();
    console.log("Database connected");
    webSocketStartup();
   
   
  } catch (err) {
    console.error(err);

    process.exit(1); // Non-zero failure code
  }
  
}

startup();

function webSocketStartup() {
  //****************** */

  //***************this snippet gets the local ip of the node.js server. copy this ip to the client side code and add ':3000' *****
  //****************exmpl. 192.168.56.1---> var sock =new WebSocket("ws://192.168.56.1:3000");*************************************
  // require('dns').lookup('Nitishs-MacBook-Air', function (err, add, fam) {
  //   console.log('addr: '+add);
  //   console.log('type : ' + os.type());
  //   console.log('type : ' + os.hostname());
  // })

  /**********************websocket setup**************************************************************************************/
  //var expressWs = require('express-ws')(app,server);
  const WebSocket = require("ws");
  const s = new WebSocket.Server({ server });

  //when browser sends get request, send html file to browser
  // viewed at http://localhost:30000
  app.get("/", function (req, res) {
    res.sendFile(path.join(__dirname + "/index.html"));
  });

  //*************************************************************************************************************************
  //***************************ws chat server********************************************************************************

  //app.ws('/echo', function(ws, req) {
  s.on("connection", function (ws, req) {
    /******* when server receives messsage from client trigger function with argument message *****/
    ws.on("message", function (message) {
      console.log("Received: " + message);
      console.log(JSON.parse(message));
      database.insertIOTData(JSON.parse(message))
      .then(
          (success)=>{
              console.log("inserted");
              s.clients.forEach(function (client) {
                //broadcast incoming message to all clients (s.clients)
                if (client != ws && client.readyState) {
                  //except to the same client (ws) that sent this message
                  client.send("broadcast: " + message);
                }
              });
          },
          (err)=>{
              throw err;
          }
      );


      
      // ws.send("From Server only to sender: "+ message); //send to client where message is from
    });
    ws.on("close", function () {
      console.log("lost one client");
    });
    //ws.send("new client connected");
    console.log("new client connected");
  });
  // server.listen(3001,'192.168.1.6');
  server.listen(3001, "192.168.1.6", () => {
    console.log("server started");
  });
}
