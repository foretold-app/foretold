module Styles = {
  open Css;
  let text = style([float(`left)]);
  let icons = style([float(`left), marginLeft(`em(0.5))]);
  let clear = style([clear(`both)]);
};

module Splitter = {
  type block =
    | Text(string)
    | Agent(string);

  type blocks = array(block);

  let agentRegExp = () => [%re "/^<agent \/>$/"];

  let toBlocks = (str: string): blocks => {
    let splitUp = Js.String.splitByRe([%re "/<split>|<\/split>/"], str);
    splitUp
    |> E.A.fmap(e =>
         switch (e) {
         | Some(str) =>
           let matching = agentRegExp() |> Js.Re.exec_(_, str);
           switch (matching) {
           | None => Some(Text(str))
           | Some(_) => Some(Agent(str))
           };
         | _ => None
         }
       )
    |> E.A.O.concatSome;
  };
};

module FeedItemMeasurableName = {
    [@react.component]
    let make = (~feedItem: Types.feedItem, ~feedItemBody: FeedItemBody.measurableB) => {
        let measurable = Primary.Measurable.ofFeedItemMeasurable(~feedItem, ~feedItemBody);
        <MeasurableItems.LinkMeasurable measurable />;
    };
};

module Columns = {
  type record = Types.feedItem;
  type column = Table.column(record);

  // @todo: To make a component.
  let name = (r: record) =>
    switch (r.body) {
    | Measurable(row) =>
      <Link
        linkType={Internal(MeasurableShow(r.channelId, row.measurableId))}>
        {row.item |> Utils.ste}
      </Link>
    | MeasurableB(row) =>
      <FeedItemMeasurableName feedItem=r feedItemBody=row/>
    | Measurement(row) =>
      <Link
        linkType={Internal(MeasurableShow(r.channelId, row.measurableId))}>
        {row.item |> Utils.ste}
      </Link>
    | JoinedMember(row) => row.item |> Utils.ste
    | Channel(row) => row.item |> Utils.ste
    | Notebook(row) => row.item |> Utils.ste
    | Series(row) => row.item |> Utils.ste
    | _ => "" |> Utils.ste
    };

  // @todo: To make a component.
  let toDescription = (str: string, r: record) => {
    str
    |> Splitter.toBlocks
    |> E.A.fmap(e =>
         switch (e) {
         | Splitter.Text(str) => str |> Utils.ste
         | Splitter.Agent(_) => <AgentLink agent={r.agent} />
         }
       )
    |> ReasonReact.array;
  };

  // @todo: To make a component.
  let description = (r: record) =>
    switch (r.body) {
    | Generic(row) => toDescription(row.description, r)
    | Measurable(row) =>
      <>
        <div className=Styles.text> {toDescription(row.description, r)} </div>
        <div className=Styles.icons>
          <Link
            className=Style.iconGray
            linkType={
              Internal(MeasurableShow(r.channelId, row.measurableId))
            }>
            <Icon icon="LINK" />
          </Link>
        </div>
      </>
    | MeasurableB(row) =>
      <>
        <div className=Styles.text> {toDescription(row.description, r)} </div>
        <div className=Styles.icons>
          <Link
            className=Style.iconGray
            linkType={
              Internal(MeasurableShow(r.channelId, row.measurableId))
            }>
            <Icon icon="LINK" />
          </Link>
        </div>
      </>
    | Measurement(row) =>
      <>
        <div className=Styles.text> {toDescription(row.description, r)} </div>
        <div className=Styles.icons>
          <MeasurementItems.Info.ById measurementId={row.measurementId} />
        </div>
      </>
    | JoinedMember(row) =>
      <>
        <div className=Styles.text> {toDescription(row.description, r)} </div>
        <div className=Styles.icons>
          <Link
            className=Style.iconGray
            linkType={Internal(ChannelShow(r.channelId))}>
            <Icon icon="LINK" />
          </Link>
        </div>
      </>
    | Channel(row) =>
      <>
        <div className=Styles.text> {toDescription(row.description, r)} </div>
        <div className=Styles.icons>
          <Link
            className=Style.iconGray
            linkType={Internal(ChannelShow(r.channelId))}>
            <Icon icon="LINK" />
          </Link>
        </div>
      </>
    | Notebook(row) =>
      <>
        <div className=Styles.text> {toDescription(row.description, r)} </div>
        <div className=Styles.icons>
          <Link
            className=Style.iconGray
            linkType={Internal(ChannelNotebook(r.channelId, row.notebookId))}>
            <Icon icon="LINK" />
          </Link>
        </div>
      </>
    | Series(row) =>
      <>
        <div className=Styles.text> {toDescription(row.description, r)} </div>
        <div className=Styles.icons>
          <Link
            className=Style.iconGray
            linkType={Internal(SeriesShow(r.channelId, row.seriesId))}>
            <Icon icon="LINK" />
          </Link>
        </div>
      </>
    | _ => "" |> Utils.ste
    };

  let channel =
    Table.Column.make(
      ~name="Community" |> Utils.ste,
      ~render=
        (r: record) =>
          <Link linkType={Internal(ChannelShow(r.channelId))}>
            {r.channel.name |> Utils.ste}
          </Link>,
      ~flex=2,
      (),
    );

  let item =
    Table.Column.make(
      ~name="Question" |> Utils.ste,
      ~render=(r: record) => r |> name,
      ~flex=3,
      (),
    );

  let description =
    Table.Column.make(
      ~name="Description" |> Utils.ste,
      ~render=(r: record) => r |> description,
      ~flex=3,
      (),
    );

  let time =
    Table.Column.make(
      ~name="Time" |> Utils.ste,
      ~render=
        (r: record) =>
          r.createdAt
          |> E.O.fmap((createdAt: MomentRe.Moment.t) =>
               createdAt |> MomentRe.Moment.fromNow(~withoutSuffix=None)
             )
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let all = [|channel, item, description, time|];
  let short = [|item, description, time|];
};


[@react.component]
let make = (~feedItems, ~columns=Columns.all) =>
  <Table columns rows=feedItems/>;