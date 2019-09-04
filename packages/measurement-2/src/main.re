type name = string;
type dog = string;
type person = {
  name,
  dog,
};

[@genType]
type persons = array(person);

[@genType]
let foo: persons = [||];