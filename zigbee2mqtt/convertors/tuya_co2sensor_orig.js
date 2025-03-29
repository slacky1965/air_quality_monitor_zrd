const {identify, reporting, ota} = require('zigbee-herdsman-converters/lib/modernExtend');

const definition = {
    fingerprint: [{modelID: 'TS0601', manufacturerName: '_TZE200_ogkdpgy2'}
                 ],
    zigbeeModel: ['TS0601'],
    model: 'Original Tuya CO2 sensor ready for update',
    vendor: 'Slacky-DIY',
    description: 'Original Tuya CO2 sensor ready for custom Firmware update',
    extend: [
      identify(),
    ],
    meta: {},
    ota: true,
};

module.exports = definition;