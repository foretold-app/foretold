open Utils;

type record = BsKen.Graph_T.T.thing;
type column = Table.column(BsKen.Graph_T.T.thing);

let nameColumn = g =>
  Table.Column.make(
    ~name="Name" |> ste,
    ~render=
      (r: record) =>
        <Link linkType={Internal(EntityShow(r |> BsKen.Graph_T.Thing.id))}>
          {r.thingId.thingIdString
           |> KenTools.Subject.name(g)
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
        r.thingId.thingIdString
        |> KenTools.Subject.instanceOf(g)
        |> E.O.default("")
        |> ste,
    (),
  );

let idColumn = g =>
  Table.Column.make(
    ~name="Name" |> ste,
    ~render=(r: record) => r |> BsKen.Graph_T.Thing.id |> ste,
    (),
  );

let all = g => [|nameColumn(g), instanceOf(g), idColumn(g)|];

[@react.component]
let make = () => {
  let g = KenTools.graphFromContext();
  <SLayout head={<SLayout.TextDiv text="All Entities" />}>
    <Table columns={all(g)} rows={KenTools.Thing.withNames(g)} />
  </SLayout>;
};