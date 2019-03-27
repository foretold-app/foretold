jest.mock('./channels');
jest.mock('./agents-channels');
jest.mock('./measurables');
jest.mock('./measurements');
jest.mock('./series');

const { middlewares } = require('./index');

const channels = require('./channels');
const agentsChannels = require('./agents-channels');
const measurables = require('./measurables');
const measurements = require('./measurements');
const series = require('./series');

describe('Middlewares', () => {

  it('measurements()', () => {
    const resolve = () => Promise.resolve(true);
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    return middlewares.Query.measurements(resolve, root, args, context, info)
      .then((result) => {
        expect(measurables.measurable)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(channels.channel)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(agentsChannels.agentsChannels)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(result).toBe(true);
      });
  });

  it('measurement()', () => {
    const resolve = () => Promise.resolve(true);
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    return middlewares.Query.measurement(resolve, root, args, context, info)
      .then((result) => {
        expect(measurements.measurement)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(measurables.measurable)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(channels.channel)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(agentsChannels.agentsChannels)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(result).toBe(true);
      });
  });

  it('measurable()', () => {
    const resolve = () => Promise.resolve(true);
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    return middlewares.Query.measurable(resolve, root, args, context, info)
      .then((result) => {
        expect(measurables.measurable)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(channels.channel)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(agentsChannels.agentsChannels)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(result).toBe(true);
      });
  });

  it('measurables', () => {
    const resolve = () => Promise.resolve(true);
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    return middlewares.Query.measurables(resolve, root, args, context, info)
      .then((result) => {
        expect(channels.channel)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(agentsChannels.agentsChannels)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(result).toBe(true);
      });
  });


  it('series()', () => {
    const resolve = () => Promise.resolve(true);
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    return middlewares.Query.series(resolve, root, args, context, info)
      .then((result) => {
        expect(series.series)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(channels.channel)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(agentsChannels.agentsChannels)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(result).toBe(true);
      });
  });

  it('seriesCollection()', () => {
    const resolve = () => Promise.resolve(true);
    const root = {};
    const args = {};
    const context = {};
    const info = {};
    return middlewares.Query
      .seriesCollection(resolve, root, args, context, info)
      .then((result) => {
        expect(channels.channel)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(agentsChannels.agentsChannels)
          .toHaveBeenCalledWith(root, args, context, info);
        expect(result).toBe(true);
      });
  });

});

