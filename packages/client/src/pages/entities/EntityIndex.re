open Utils;

module Entities = {
  type record = KenTools.Thing.t;
  type column = Table.column(KenTools.Thing.t);
  let nameColumn = g =>
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (r: record) =>
          <Link linkType={Internal(EntityShow(r |> KenTools.Thing.id))}>
            {KenTools.Thing.id(r)
             |> KenTools.Subject.name(g)
             |> E.O.bind(_, KenTools.FactValue.toString)
             |> E.O.default("")
             |> ste}
          </Link>,
      ~flex=2,
      (),
    );

  let instanceOf = g =>
    Table.Column.make(
      ~name="Instance Of" |> ste,
      ~render=
        (r: record) =>
          switch (
            KenTools.Subject.instanceOf(g, KenTools.Thing.id(r))
            |> E.O.bind(_, KenTools.FactValue.thingId)
          ) {
          | Some(id) =>
            <Link linkType={Internal(EntityShow(id))}>
              {id
               |> KenTools.Subject.name(g)
               |> E.O.bind(_, KenTools.FactValue.toString)
               |> E.O.default("")
               |> Utils.ste}
            </Link>
          | None => ReasonReact.null
          },
      (),
    );

  let idColumn = g =>
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=(r: record) => r |> KenTools.Thing.id |> ste,
      (),
    );

  let all = g => [|nameColumn(g), instanceOf(g), idColumn(g)|];
};

module Facts = {
  type record = KenTools.Fact.fact;
  type column = Table.column(KenTools.Fact.fact);
  let nameColumn = g =>
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (r: record) =>
          <Link linkType={Internal(EntityShow(r.subjectId))}>
            {r.subjectId
             |> KenTools.Subject.name(g)
             |> E.O.bind(_, KenTools.FactValue.toString)
             |> E.O.default("")
             |> ste}
          </Link>,
      ~flex=2,
      (),
    );

  let propertyOf = g =>
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (r: record) =>
          <Link linkType={Internal(EntityShow(r.propertyId))}>
            {r.propertyId
             |> KenTools.Subject.name(g)
             |> E.O.bind(_, KenTools.FactValue.toString)
             |> E.O.default("")
             |> ste}
          </Link>,
      ~flex=2,
      (),
    );

  let valueColumn = g =>
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (r: record) =>
          switch (KenTools.Fact.value(r).valueType) {
          | String(s) => s |> Utils.ste
          | ThingId(s) => KenTools.Subject.nameWithLink(g, s)
          | _ => ReasonReact.null
          },
      (),
    );

  let all = g => [|nameColumn(g), propertyOf(g), valueColumn(g)|];
};

[@react.component]
let make = () => {
  let g = KenTools.Graph.fromContext();
  <SLayout head={<SLayout.TextDiv text="All Entities" />}>
    <h2> {"Entities" |> Utils.ste} </h2>
    <Table columns={Entities.all(g)} rows={KenTools.Thing.withNames(g)} />
    <h2> {"Facts" |> Utils.ste} </h2>
    <Table
      columns={Facts.all(g)}
      rows={KenTools.Fact.all(g) |> Array.of_list}
    />
  </SLayout>;
};