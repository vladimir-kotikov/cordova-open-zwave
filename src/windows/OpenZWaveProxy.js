
function FakeProxy() {
    this.start = function (port, success, error) {
        setTimeout(function () { success('driverReady'); }, 2000);

        setTimeout(function () { success('nodeAdded', '1008', '564'); }, 4000);
        setTimeout(function () { success('nodeAdded', '1028', '564'); }, 5000);
        setTimeout(function () { success('nodeAdded', '1567', '580'); }, 3000);
        setTimeout(function () {
            success('nodeInfo', '1028', '564', {
                manufacturer: 'ManufacturerOne',
                manufacturerid: 'ManufacturerIdOne',
                product: 'SomeProduct',
                producttype: 'MaybeButton',
                productid: 'KHAKHFASUIHFKSFHKSHFJA',
                type: 'Button',
                name: 'Button_KHAKHFASUIHFKSFHKSHFJA',
                loc: '0.343243242356354'
            });
        }, 8000);
        setTimeout(function () {
            success('nodeInfo', '1008', '564', {
                manufacturer: 'ManufacturerTwo',
                manufacturerid: 'ManufacturerIdTwo',
                product: 'SomeOtherProduct',
                producttype: 'MaybeSwitch',
                productid: 'LJASDOAISDOLAJDLAKSDJ',
                type: 'Switch',
                name: 'Switch_LJASDOAISDOLAJDLAKSDJ',
                loc: 'Hall'
            });
        }, 10000);
        setTimeout(function () { success('nodeRemoved', '1567', '580'); }, 12000);
    }
}

var proxyInstance;

var proxy = {
    connect: function (succcess, error, args) {
        if (proxyInstance) {
            error(new Error('zwave already initialized'));
            return;
        }

        var nodes = {};

        var port = args[0];
        // This param is for testing purposes only
        var useFakeProxy = !!args[1];

        proxyInstance = useFakeProxy ? new FakeProxy() : new OZWProxy.Proxy();
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
