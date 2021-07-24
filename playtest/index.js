const net = require('net');
const port = 12069;

const server = new net.Server();

server.listen(port, () => console.log(`Listening to port ${port}...`));

let packet_types = {
    1: (d) => console.log(d.toString()),
    21: (data) => {
        if (data.byteLength < 15) return;
        let time = data.readDoubleLE(2);
        let x = data.readFloatLE(10);
        let y = data.readFloatLE(14);

        console.log(`[ ${time}s ] X: ${x}, Y: ${y}`);
    },
    22: (data) => console.log("PLAYER DESTROYED")
}

let got_size = false;
let packet_size = 0;

let bytes_available = Buffer.alloc(0);

function ondata(chunk) {
    let bytes = Buffer.byteLength(chunk);

    if (bytes > 0)
        bytes_available = Buffer.concat([bytes_available, chunk]);

    if (!got_size && Buffer.byteLength(bytes_available) >= 4) {
        packet_size = bytes_available.readUInt32LE(0);
        got_size = true;

        bytes_available = bytes_available.slice(4);
    }

    if (got_size && Buffer.byteLength(bytes_available) >= packet_size) {
        let packet_data = bytes_available.slice(0, packet_size);
        bytes_available = bytes_available.slice(packet_size);

        got_size = false;

        let id = packet_data.readUInt16LE(0);

        if (packet_types[id]) packet_types[id](packet_data);
    }
}

server.on('connection', (socket) => {
    console.log('New client');

    socket.on('data', ondata);
    
    socket.on('end', function() {
        console.log('CLIENT CLOSED');
    });

    socket.on('error', function(err) {
        console.log(`Error: ${err}`);
    });
});

setInterval(ondata, 10, Buffer.alloc(0));