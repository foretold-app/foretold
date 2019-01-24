open Utils;
open Rationale;
open PrimaryTableBase;

let compareSimilarMeasurables =
    (measurableA: DataModel.measurable, measurableB: DataModel.measurable) =>
  switch (
    measurableA.expectedResolutionDate,
    measurableB.expectedResolutionDate,
  ) {
  | (Some(a), Some(b)) => MomentRe.Moment.isAfter(a, b) ? 1 : (-1)
  | _ => 1
  };

let compareMeasurables =
    (measurableA: DataModel.measurable, measurableB: DataModel.measurable) =>
  switch (status(measurableA), status(measurableB)) {
  | (a, b) when a == b => compareSimilarMeasurables(measurableA, measurableB)
  | (a, b) => statusInt(a) > statusInt(b) ? (-1) : 1
  };

let formatDate = e =>
  Option.Infix.(e <$> MomentRe.Moment.format("L") |> Option.default(""));

let dateFinder = (~measurable: DataModel.measurable, head, p, date) => {
  let date = formatDate(date);
  <div className=PrimaryTableStyles.statusRow>
    <h3> {head |> ste} </h3>
    <p> {p ++ date |> ste} </p>
  </div>;
};

let dateStatus = (~measurable: DataModel.measurable) => {
  let m = measurable;
  switch (status(m)) {
  | OPEN =>
    dateFinder(~measurable=m, "Open", "Closes ~", m.expectedResolutionDate)
  | PENDING_REVIEW =>
    dateFinder(
      ~measurable=m,
      "Judgement Pending",
      "Pending since ",
      m.expectedResolutionDate,
    )
  | CLOSED => dateFinder(~measurable=m, "Closed", "Closed on ", m.lockedAt)
  };
};

let dateStatusWrapper = (~measurable: DataModel.measurable) =>
  <div className={PrimaryTableStyles.statusColor(~measurable)}>
    {dateStatus(~measurable)}
  </div>;

let sortMeasurables = m => Belt.SortArray.stableSortBy(m, compareMeasurables);

let link = (~m: DataModel.measurable) =>
  <a href={"/measurables/" ++ m.id}> {m.name |> ste} </a>;

let description = (~m: DataModel.measurable) =>
  switch (m.description |> Option.default("")) {
  | "" => <div />
  | text => <p> {text |> ste} </p>
  };

let isLocked = (~m: DataModel.measurable) =>
  switch (m.isLocked) {
  | true =>
    <div className=PrimaryTableStyles.item>
      <span> {"Locked on " |> ste} </span>
      <span> {m.lockedAt |> formatDate |> ste} </span>
    </div>
  | _ => <div />
  };

let creatorLink = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    Option.Infix.(
      m.creator
      <$> (
        c =>
          <a href={"/agents/" ++ c.id}>
            {c.name |> Option.default("") |> ste}
          </a>
      )
      |> Option.default("" |> ste)
    )
  </div>;

let editLink = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    <a href={"/measurables/" ++ m.id ++ "/edit"}> {"Edit" |> ste} </a>
  </div>;

let measurements = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    <span>
      {m.measurementCount |> Option.default(0) |> string_of_int |> ste}
    </span>
    <span> {" measurements" |> ste} </span>
  </div>;

let measurers = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    <span>
      {m.measurerCount |> Option.default(0) |> string_of_int |> ste}
      <span> {" measurers" |> ste} </span>
    </span>
  </div>;

let expectedResolutionDate = (~m: DataModel.measurable) =>
  <div className=PrimaryTableStyles.item>
    <span> {"Resolves on " |> ste} </span>
    <span> {m.expectedResolutionDate |> formatDate |> ste} </span>
  </div>;

let resolutionEndpoint = (~m: DataModel.measurable) =>
  switch (m.resolutionEndpoint |> Option.default("")) {
  | "" => <div />
  | text =>
    <div className=PrimaryTableStyles.item>
      <span> <span> {"Endpoint: " |> ste} </span> {text |> ste} </span>
    </div>
  };