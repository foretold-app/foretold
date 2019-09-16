var {toMeasurementValue, score, fromOption, getScoreOverTime, itemArray} = require('./src/Utility/JsExport.gen');

test('adds 1 + 2 to equal 3', () => {
    expect((toMeasurementValue({data: 0.2, dataType: "percentage"}))).toBe(3);
  });

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

test('adds 2 + 2 to equal 4', () => {
    expect((itemArray([
        {time: 3.0, measurement: {data: 0.2, dataType: "percentage"}},
        {time: 3.0, measurement: {data: 0.2, dataType: "percentage"}},
        {time: 3.0, measurement: {data: 0.2, dataType: "percentage"}},
    ]))).toBe(3);
  });

test('adds 2 + 2 to equal 4', () => {
    expect((getScoreOverTime(
      [{time: 3.0, measurement: {data: 0.1, dataType: "percentage"}},
      {time: 8.0, measurement: {data: 0.4, dataType: "percentage"}},
      {time: 11.0, measurement: {data: 0.8, dataType: "percentage"}}
    ],
      [
        {time: 4.0, measurement: {data: 0.91, dataType: "percentage"}}
      ],
      {time: 12.0, measurement: {data: 0.1, dataType: "percentage"}}
    ))).toBe(3);
  });
