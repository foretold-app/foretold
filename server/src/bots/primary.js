const models = require("../models")
const Sequelize = require('sequelize')
const _ = require("lodash")
const Op = Sequelize.Op;

class Evaluation {
  constructor({
    previousMeasurements,
    evaluateFn
  }) {
    this.previousMeasurements = previousMeasurements;
    this.evaluateFn = evaluateFn;
  }

  async getPreviousMeasurements(measurement) {
    let measurements = await models.Measurement.findAll({
      where: {
        measurableId: measurement.dataValues.measurableId,
        competitorType: "COMPETITIVE",
        createdAt: {
          [Op.lt]: measurement.dataValues.createdAt
        }
      }
    })
    this.previousMeasurements = [measurement, ...measurements];
  }

  run() {
    return this.evaluateFn(this.previousMeasurements.map(m => m.dataValues))
  }
}

class Bot {
  constructor({
    botId,
    agentId,
    evaluateFn
  }) {
    this.botId = botId;
    this.agentId = agentId;
    this.evaluateFn = evaluateFn;
  }

  async processAllMeasurements(){
    let measurements = await models.Measurement.findAll({
      where: {
        competitorType: "COMPETITIVE",
      }
    });
    for (const m of measurements){
        await this.createAggregateMeasurement(m)
    }
  }

  async createAggregateMeasurement(measurement) {
    let newVal = await this.getValue(measurement);
    let previousMeasurement = await models.Measurement.findAll({
      where: {
        agentId: this.agentId,
        taggedMeasurementId: measurement.dataValues.id
      }
    })

    if (previousMeasurement.length > 0) {
      console.log("Already exists")
      return previousMeasurement
    } else {
      let newMeasurement = await models.Measurement.create({
        agentId: this.agentId,
        competitorType: "AGGREGATION",
        measurableId: measurement.dataValues.measurableId,
        taggedMeasurementId: measurement.id,
        relevantAt: measurement.createdAt,
        value: newVal
      })
        return newMeasurement;
    }

  }

  async getValue(measurement) {
    let evaluation = new Evaluation({
      evaluateFn: this.evaluateFn
    })
    await evaluation.getPreviousMeasurements(measurement)
    return evaluation.run()
  }

}

let evaluate = function (vs) {
  let foo = vs.filter(v => v.value.dataType == "floatPercentiles");
  return {
    dataType: "floatPercentiles",
    data: {
      "25.": _.mean(foo.map(e => e.value.data["25."])) * 0.7,
      "50.": _.mean(foo.map(e => e.value.data["50."])),
      "75.": _.mean(foo.map(e => e.value.data["75."])) * 1.3
    }
  }
}

let bot1 = new Bot({
  botId: "1010a04d-5307-4bd0-9246-000000000000",
  agentId: "b8c2e25e-e019-4307-a4a9-704589ff6048",
  evaluateFn: evaluate
})

let foo = async function () {
  try {
    let measurement = await models.Measurement.findById("3c93c859-9676-4bc5-a9af-3b34a811c8a9")
    await bot1.processAllMeasurements()
  } catch (e) {
    console.log(e)
  }
}

foo()
