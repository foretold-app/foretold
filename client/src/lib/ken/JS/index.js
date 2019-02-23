let foo = "bar";
import * as gitNet from "./Interface.gen"
import * as R from "ramda";

export class Value {
    constructor(value, fact, db){
        this.value = value;
        this.fact = fact;
        this.db = db;
        return this;
    }
    json() {
        return gitNet.Value_to_json(this.value);
    }
    dataType(){
        return this.json().dataValue
    }
    data(){
        return this.json().data
    }
    thing(){
        if (this.dataType() !=="thingId"){ return false }
        return this.db.findThing(this.data())
    }
}

export class FactList {
    constructor(db, factList=[]){
        this.db = db;
        this.factList = factList;
        return this;
    }
    filter(query){
        if (this.factList.length == 0){
            this.factList = gitNet.Graph_factList(this.db.graph);
        }
        let _query = gitNet.Query_from_json(query);
        let list = gitNet.Filter_query(_query,this.factList)
        return (new FactList(this.db, list))
    }
    facts(){
        let ps = gitNet.list_to_array(this.factList);
        return ps.map(e => new Fact(e, this.db))
    }
}

export class Fact {
    constructor(fact, db){
        this.fact = fact;
        this.db = db;
        return this;
    }
    json() {
        return gitNet.Fact_to_json(this.fact)
    }
    id() {
        return this.json().id;
    }
    internalThing(){
        return this.db.findThing(this.id())
    }
    thing(edge){
      let thing = gitNet.Graph_findThingFromFact(this.db.graph, edge, this.fact);
      return new Thing(thing, this.db);
    }
    property(){
        return this.thing(gitNet.Graph_EdgeTypes_property)
    }
    subject(){
        return this.thing(gitNet.Graph_EdgeTypes_subject)
    }
    value(){
        return new Value(gitNet.Fact_value(this.fact), this, this.db);
    }
}

export class Thing {
    constructor(thing, db){
        this.thing = thing;
        this.db = db;
        return this;
    }
    json(){
        return gitNet.Thing_to_json(this.thing)
    }
    id() {
        return this.json().id;
    }
    propertyIdFacts(id){
        return this.isEdge("SUBJECT").filter({id: id, edge: "PROPERTY", q: ""}).facts();
    }
    isEdge(edge){
        return (new FactList(this.db)).filter({id: this.json().id, edge, q: ""})
    }
    isSubjectForFacts(){
        return this.isEdge("SUBJECT").facts()
    }
    isPropertyForFacts(){
        return this.isEdge("PROPERTY").facts()
    }
    isValueForFacts(){
        return this.isEdge("VALUE").facts()
    }
    connectedPropertyThings(){
        return this.isEdge("SUBJECT").facts().map(f => f.property())
    }
    _byProperty(edge){
        let facts = this.isEdge(edge);
        let props = facts.facts().map(f => f.property());
        let properties = R.uniqBy(r => r.id(), props);
        let bunch = properties.map(property => ({
            property: property,
            facts: facts.filter({id: property.json().id, edge: "PROPERTY", q: ""}).facts()
        }))
        return bunch
    }
    isSubjectForFactsByProperty(){
        return this._byProperty("SUBJECT");
    }
    isValueForFactsByProperty(){
        return this._byProperty("VALUE");
    }
}

export class Database {
    constructor(data){
        this.graph = gitNet.Graph_fromJson(data)
        console.log("DB!", this.graph)
        return this;
    }
    findThing(id){
      let thing = gitNet.Graph_findThing(id, this.graph);
      if (!!thing){
      return new Thing(thing, this);
      } else {
          return false
      }
    }
    directories(){
        return gitNet.Graph_directoryArray(this.graph);
    }
    childDirectories(s){
        return gitNet.Graph_childDirectories(this.graph, s);
    }
    rootDirectories(){
        return gitNet.Graph_rootDirectories(this.graph);
    }
    things(){
        return gitNet.Graph_things(this.graph).filter(t => !!t).map(t => new Thing(t, this))
    }
    json(){
        return gitNet.Graph_to_json(this.graph)
    }
}

export function main(data){
    return new Database(data);
}
