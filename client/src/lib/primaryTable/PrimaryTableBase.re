type status =
  | OPEN
  | PENDING_REVIEW
  | CLOSED;

let statusInt = (status: status) =>
  switch (status) {
  | OPEN => 2
  | PENDING_REVIEW => 1
  | CLOSED => 0
  };

let status = (measurable: Queries.measurable) => {
  let pastExpectedResolutionDate =
    switch (measurable.expectedResolutionDate) {
    | None => false
    | Some(e) => MomentRe.Moment.isAfter(MomentRe.momentNow(), e)
    };
  if (!measurable.isLocked && pastExpectedResolutionDate) {
    PENDING_REVIEW;
  } else if (measurable.isLocked) {
    CLOSED;
  } else {
    OPEN;
  };
};