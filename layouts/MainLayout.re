[@react.component]
let make = (~children) => {
  let minWidth = ReactDOMRe.Style.make(~minWidth="20rem", ());
  <div style=minWidth className="flex">
    <div className="lg:w-screen text-gray-900 font-base">
      <main className="w-screen"> children </main>
    </div>
  </div>;
};
