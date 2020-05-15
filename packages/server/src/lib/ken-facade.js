const _ = require('lodash');

const kenFacade = require('bs-ken/dist');


/**
 * Subject
 * @foretold/main/n-foretold
 * Name --> Foretold
 *
 * Property (value)
 * @foretold/main/measurables
 * Name --> # of Measurables
 */
class KenFacade {
  constructor(entityGraph) {
    this.db = kenFacade.main(entityGraph);
    this.NAME = '@base/properties/p-name';
  }

  /**
   * subjectId = @foretold/main/n-foretold
   * propertyId = @foretold/main/measurables
   * @param {string} subjectId
   * @param {string} propertyId
   * @returns {{subject: string, property: string}}
   */
  names(subjectId, propertyId) {
    return {
      subject: this.name(subjectId),
      property: this.name(propertyId),
    };
  }

  /**
   * id = @foretold/main/n-foretold
   * @param {string} id
   * @returns {string}
   */
  name(id) {
    const result = this.db.findThing(id);
    const things = result
      ? result.propertyIdFacts(this.NAME)
      : [];
    const thing = _.head(things);
    return this.value(thing);
  }

  /**
   * @param thing
   * @returns {string|*}
   */
  thingProperty(thing) {
    if (!thing) return '';
    const facts = thing.propertyIdFacts(this.NAME);
    const datas = facts.map((e) => e.value().data());
    const data = _.head(datas);
    return thing.id() + data;
  }

  /**
   * @param fact
   * @returns {string|string|*}
   */
  value(fact) {
    if (!fact) return '';

    const value = fact.value();
    const type = value.dataType();

    if (type === 'string') {
      return value.data();
    }

    if (type === 'thingId') {
      const thing = value.thing();
      return this.thingProperty(thing);
    }

    return '';
  }
}

module.exports = {
  KenFacade,
};
