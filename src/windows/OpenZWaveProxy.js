var proxyInstance;

var proxy = {
    connect: function (succcess, error, args) {
        if (proxyInstance) {
            error(new Error('zwave already initialized'));
            return;
        }

        var nodes = {};

        var port = args[0];
        proxyInstance = new OZWProxy.Proxy();
        proxyInstance.start(port, function (status, nodeId, homeId) {
            if (status === 'driverReady') {
                console.log('Driver ready');
            }

            if (status === 'nodeAdded') {
                nodes[nodeId] = homeId;
            }

            if (status === 'nodeRemoved' && nodes[nodeId]) {
                delete nodes[nodeId];
            }

            var result = Object.keys(nodes)
                .map(function (nodeId) {
                    return { nodeId: nodeId, homeId: nodes[nodeId] };
                });

            succcess(result, { keepCallback: true });
        }, error);
    }
};

require('cordova/exec/proxy').add('OpenZWave', proxy);
