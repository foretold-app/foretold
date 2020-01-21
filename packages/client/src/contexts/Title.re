let main = () => Lang.Title.main;
let sub = (breadcrumbs: array(string)) =>
  breadcrumbs |> Js.Array.joinWith(Lang.Title.breadcrumbsSeparation);