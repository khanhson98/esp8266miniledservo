var PORT = process.env.PORT || 5000;
var express = require('express');
var app = express();
app.use(express.static("public"));
app.set("view engine", "ejs");
app.set("views", "./views");
var http = require('http');
var server = http.Server(app);
var count = 0;

var io = require('socket.io')(server);
server.listen(PORT, function () {
    console.log('listening on *:' + PORT);
}
);

io.on('connect',function(socket){
	console.log('a user connected!');
	socket.on('disconnect', function(){
              console.log('user disconnected');
	});

    socket.on("colorChanged", function(data){
        console.log(data);
        socket.broadcast.emit("colorChanged",data);
    });
});


app.get('/', (req, res) => {
    res.render('trangchu');
});