const path = require('path');
const express = require('express');
const { Server } = require('ws');
//const app = express();

//const WS_PORT  =  process.env.PORT || 8888;
const HTTP_PORT = process.env.PORT || 8000;
const INDEX = '/client.html';
//const HTTP_PORT = 80;

const server = express()
  .use((req, res) => res.sendFile(INDEX, { root: __dirname }))
  .listen(HTTP_PORT, () => console.log(`Listening on ${HTTP_PORT}`));

  const wsServer = new Server({ server });
//const wsServer = new WebSocket.Server({port: WS_PORT}, ()=> console.log(`WS Server is listening at ${WS_PORT}`));

let connectedClients = [];
let count = 0;
wsServer.on('connection', (ws, req)=>{
    console.log('Connected');
    connectedClients.push(ws);

    ws.on('message', data => {
       connectedClients.forEach((ws,i)=>{
            if(ws.readyState === ws.OPEN){
                ws.send(data);
               // ws.clients.forEach(
               //     function(client) {
               //         
               //         client.send(data);
               //      
               //     });
                console.log("ESP send " + count + " length = " + connectedClients.length);
                count++;
            }else{
                //connectedClients.splice(i ,1);
            }
        })
    });

    // handling what to do when clients disconnects from server
    ws.on("close", () => {
        console.log("the client has disconnected");
    });
    // handling client connection error
    ws.onerror = function () {
        console.log("Some Error occurred");
    };
});

    


//app.get('/client',(req,res)=>res.sendFile(path.resolve(__dirname, './client.html')));
//app.listen(HTTP_PORT, ()=> console.log(`HTTP server listening at ${HTTP_PORT}`));