open HandsOnTable;

module Columns = {
  let standard = x => makeColumn(~name=x, ());
  let html = x => makeColumn(~name=x, ~renderer="html", ());
  let checkbox = x => makeColumn(~name=x, ~renderer="checkbox", ());
};

module ColumnBundle = {
  type t('a) = {
    name: string,
    get: 'a => string,
    column: string => HandsOnTable.column,
    headerName: string,
  };

  type l('a) = list(t('a));

  let make = (~headerName, ~get, ~column=Columns.standard, ()) => {
    name: headerName,
    get,
    column,
    headerName,
  };

  let toDict = (items: l('a), e: 'a) =>
    items |> List.map((r: t('a)) => (r.name, r.get(e))) |> Js.Dict.fromList;

  let toColumns = (items: l('a)) =>
    items |> List.map((r: t('a)) => r.column(r.name)) |> Array.of_list;

  let toHeaders = (items: l('a)) =>
    items |> List.map((r: t('a)) => r.headerName) |> Array.of_list;

  let toHOT = (~data: array('a), ~transformations: l('a)) => {
    let data = data |> Array.map(e => e |> (transformations |> toDict));
    <UseRouterForLinks>
      <HandsOnTable
        data
        columns=(transformations |> toColumns)
        colHeaders=(transformations |> toHeaders)
      />
    </UseRouterForLinks>;
  };
};