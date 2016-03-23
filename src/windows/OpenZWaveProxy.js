
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
        proxyInstance.start(port, function (status, nodeId, homeId, nodeInfo) {

            switch (status) {
                case 'driverReady':
                    console.log('Driver ready');
                    break;
                case 'nodeAdded':
                    nodes[nodeId] = { nodeId: nodeId, homeId: homeId };
                    break;
                case 'nodeRemoved':
                    if (nodes[nodeId]) delete nodes[nodeId];
                    break;
                case 'nodeInfo':
                    if (!nodes[nodeId]) nodes[nodeId] = { nodeId: nodeId, homeId: homeId };
                    Object.keys(nodeInfo).forEach(function (infoKey) {
                        nodes[nodeId][infoKey] = nodeInfo[infoKey];
                    });
                    break;
            }

            var result = Object.keys(nodes)
            .map(function (nodeId) {
                return nodes[nodeId];
            });

            succcess(result, { keepCallback: true });
        }, function (err) {
            error(new Error(err));
        });
    },
    stop: function (success, error, args) {
        if (proxyInstance) {
            proxyInstance.destroy();
            proxyInstance = null;
        }
    }
};

require('cordova/exec/proxy').add('OpenZWave', proxy);
