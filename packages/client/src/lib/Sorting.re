type sortable = Types.channel;

let sortDefault = (arr: array('a)) => arr;

let sortAsc = (arr: array(sortable)) => {
  Array.sort(
    (a: sortable, b: sortable) => String.compare(a.name, b.name),
    arr,
  );
  arr;
};