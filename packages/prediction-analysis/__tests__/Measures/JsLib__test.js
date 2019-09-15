
var {MeasurementValue} = require("../../src/JsExport/JsLib");

test('adds 1 + 2 to equal 3', () => {
    expect((new MeasurementValue({data: 0.2, dataType: "percentage"}))).toBe(3);
  });