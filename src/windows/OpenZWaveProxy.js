
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
                    nodes[nodeId] = { homeId: homeId };
                    break;
                case 'nodeRemoved':
                    if (nodes[nodeId]) delete nodes[nodeId];
                    break;
                case 'nodeInfo':
                    Object.keys(nodeInfo).forEach(function (infoKey) {
                        node[nodeId][infoKey] = nodeInfo[infoKey];
                    });
                    break;
            }

            var result = Object.keys(nodes)
            .map(function (nodeId) {
                return { nodeId: nodeId, homeId: nodes[nodeId] };
            });

            succcess(result, { keepCallback: true });
        }, function (err) {
            proxy.stop();
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
