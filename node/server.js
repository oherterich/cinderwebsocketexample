var express = require('express'); //used for easily creating servers
var WebSocketServer = require('ws').Server; //used for connecting with websockets

//Start our server
var app = express(); 
var server = require('http').createServer(app);

var WWW_ROOT = '/public'; //we'll find our client-side web pages in the public folder

//Randomly picked two ports - one to host the website, one to send websocket data through
var HTTP_PORT = 8080;
var WEBSOCKET_PORT = 8081;

var CINDER; //will use this later to send data specifically to our cinder app

var wss = new WebSocketServer({ port: WEBSOCKET_PORT }); //create a new websocket server listening to port 8081

app.use(express.static(__dirname + WWW_ROOT)); //this line basically allows us to view our web pages in the browser

app.listen(HTTP_PORT); //server will listen on port 8080 (note: this is separate from websockets)

//If something has connected to our websocket port, do whatever is inside this function.
wss.on('connection', function(ws) {
	console.log('Connected!');

	//ws.on('message') is a built-in function. it triggers when data is sent through the socket.
	ws.on('message', function(msg) {
		/*
		This is slightly confusing. There should be a good way to separate values when you send messages through sockets.
		A good way is usually JSON. For example: { "message": "hi" }. 
		I was having trouble with JSON in Cinder, so for now it's using CSV.
		The first part of the message is always the "event". 
		*/

		var parts = msg.split(",");
		var event = parts[0];
		console.log(parts);

		//If Cinder has sent us a message, assign that particular connection to the CINDER variable.
		if ( event == 'init-cinder' ) {
			CINDER = ws;
			ws.send( "init-cinder" );
		}

		//If we get an event for 'change-pos' (from the browser) you should send that message along to Cinder.
		if ( event == 'change-pos' ) {
			if (CINDER) {
				CINDER.send(msg);
			}
		}
	});
});