var {toMeasurementValue, score, fromOption} = require('../../src/Utility/JsExport.gen');

// test('adds 1 + 2 to equal 3', () => {
//     expect((toMeasurementValue({data: 0.2, dataType: "percentage"}))).toBe(3);
//   });

test('adds 2 + 2 to equal 3', () => {
    expect((score({data: 0.2, dataType: "percentage"},
        {data: 0.5, dataType: "percentage"},
    {data: 0.1, dataType: "percentage"}
    ).data)).toBe([.47807]);
  });

test('adds 2 + 2 to equal 3', () => {
    expect((score({data: {xs: [1., 2.,3.,4.,5.], ys: [.0, .0, .0, .99, .99]}, dataType: "floatCdf"},
        {data: {xs: [1., 2., 3., 4., 5.] , ys: [.0, .0, .99, .99, .99]}, dataType: "floatCdf"},
        {data: {xs: [1.,2.,3.,4.,5.], ys: [.0, .0, .99, .99, .99]}, dataType: "floatCdf"},
    ))).toBe(3);
  });
