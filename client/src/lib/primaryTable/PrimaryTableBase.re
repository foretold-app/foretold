type status =
  | OPEN
  | PENDING_REVIEW
  | ARCHIVED
  | JUDGED;

/* TODO: CHange closed to judged */

let statusInt = (status: status) =>
  switch (status) {
  | OPEN => 3
  | PENDING_REVIEW => 2
  | JUDGED => 1
  | ARCHIVED => 0
  };

let status = (measurable: DataModel.measurable) => {
  let state =
    measurable.state |> E.O.toExn("Needs state from GraphQL");
  if (state === `ARCHIVED) {
    ARCHIVED;
  } else if (state === `JUDGED) {
    JUDGED;
  } else {
    let pastExpectedResolutionDate =
      switch (measurable.expectedResolutionDate) {
      | None => false
      | Some(e) => MomentRe.Moment.isAfter(MomentRe.momentNow(), e)
      };
    if (pastExpectedResolutionDate) {PENDING_REVIEW} else {OPEN};
  };
};