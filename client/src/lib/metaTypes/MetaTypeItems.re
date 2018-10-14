open MetaTypeBase;

module Generic = {
  module In = {
    let id = "1";
    let name = "Generic";
    let description = "General Tasks";
    type t = {
      name: string,
      description: string,
    };
    let metaTypes = [|("name", `String), ("description", `String)|];
  };
  module Out = MakeMeasurableType(In);
};

module TotalRepoCommits = {
  module In = {
    let id = "2";
    let name = "TotalRepoCommits";
    let description = "The total commits for a repo at a specific time.";
    type t = {
      byDate: Js.Date.t,
      repo: entity,
    };
    let metaTypes = [|("byDate", `Date), ("repo", `Entity)|];
  };
  module Out = MakeMeasurableType(In);
};

/* //Type is float */
module HumanTaskTime = {
  module In = {
    let id = "3";
    let name = "HumanTaskTime";
    let description = "The time it takes to finish a task, in milliseconds.";
    type t = {
      task: entity,
      organization: option(entity),
      person: option(entity),
    };
    let metaTypes = [|
      ("task", `Entity),
      ("organization", `String),
      ("person", `String),
    |];
  };
  module Out = MakeMeasurableType(In);
};

/* //Type is dateTime */
module ExpectedYearOfDeath = {
  module In = {
    let id = "4";
    let name = "ExpectedYearOfDeath";
    let description = "The time it takes to finish a task, in milliseconds.";
    type t = {entity};
    let metaTypes = [|("byDate", `Date), ("repo", `Entity)|];
  };
  module Out = MakeMeasurableType(In);
};

/* //Type is dateTime */
module TimeOfFirstJudgement = {
  module In = {
    let id = "5";
    let name = "TimeOfFirstJudgement";
    let description = "The time it takes to finish a task, in milliseconds.";
    type t = {measurableId: string};
    let metaTypes = [|("byDate", `Date), ("repo", `Entity)|];
  };
  module Out = MakeMeasurableType(In);
};

module ProbabilityOfJudgement = {
  module In = {
    let id = "6";
    let name = "ProbabilityOfJudgement";
    let description = "The time it takes to finish a task, in milliseconds.";
    type t = {
      measurableId: string,
      evaluationTime: string,
    };
    let metaTypes = [|("byDate", `Date), ("repo", `Entity)|];
  };
  module Out = MakeMeasurableType(In);
};

module UsefulnessRubricOnMeasurement = {
  module In = {
    let id = "7";
    let name = "UsefulnessRubricOnMeasurement";
    let description = "The time it takes to finish a task, in milliseconds.";
    type t = {
      measurableId: string,
      evaluationTime: string,
    };
    let metaTypes = [|("byDate", `Date), ("repo", `Entity)|];
  };
  module Out = MakeMeasurableType(In);
};