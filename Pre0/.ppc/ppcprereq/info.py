from ppcgrader.info_utils import *
from ppcgrader.doc_builder import *
from ppcgrader.reporter import json_to_output

code = "prereq"
name = "Pre"
descr = "prerequisite test"


def explain_web(raw: dict):
    return generate_html(explain(json_to_output(raw), "web"))


def explain_terminal(r, color=False):
    printer = TerminalPrinter(color=color)
    return generate_term(explain(r, "term"), printer)


def explain(r, mode: str):
    fail_style = "verywrong"
    if mode == "web":
        fail_style = "strong"  # preserve old web emphasis

    domain = get_domain(mode=mode, domain=code)

    builder = DocumentBuilder(mode)
    input = r.input_data or {}
    output = r.output_data or {}
    oe = r.output_errors or {}

    error_magnitude = oe.get("error_magnitude", None)
    threshold = oe.get("threshold", None)

    par = ["ny", "nx", "y0", "x0", "y1", "x1"]
    if all(x in input for x in par):
        with builder.paragraph() as txt:
            txt += domain.gettext("function_called_with_params")
        with builder.list(style="compact") as lst:
            for x in par:
                with lst.item() as item:
                    item += f"{x} = {input[x]}"

        ny = input.get("ny", None)
        nx = input.get("nx", None)

        if input.get("data",
                     None) is not None and ny is not None and nx is not None:
            with builder.paragraph() as txt:
                txt += domain.gettext("input_data_with_color_components")
            with builder.matrix(ny, nx) as mat:
                data = input.get("data", None)
                for i in range(ny):
                    for j in range(nx):
                        mat.entry(
                            i,
                            j,
                            TextNode([
                                StringNode(safeprint(safeget(data, i, j, 0)),
                                           style="vector"),
                                StringNode(safeprint(safeget(data, i, j, 1)),
                                           style="vector"),
                                StringNode(safeprint(safeget(data, i, j, 2)),
                                           style="vector"),
                            ]),
                            "vector",
                        )

    if "avg" in output:
        with builder.paragraph() as txt:
            txt += domain.gettext("produced_result")
            if r.errors:
                txt += domain.gettext("result_was_prefix")
                txt += StringNode(domain.gettext("not_correct"), fail_style)
            txt += ":"

        avg = output.get("avg", [])
        with builder.list(style="compact") as lst:
            for i in range(3):
                with lst.item() as item:
                    item += f"avg[{i}] = {safeprint(safeget(avg, i))}"

    if safenum(error_magnitude) > 0 and safenum(threshold) > 0:
        with builder.paragraph() as txt:
            txt += domain.gettext("largest_error_intro")
            txt += strong(safereadable(error_magnitude))
            txt += domain.gettext("largest_error_limit",
                                  threshold=safereadable(threshold))
            txt += domain.gettext(
                "largest_error_ratio",
                error_ratio=safereadable(
                    safenum(error_magnitude) / safenum(threshold)),
            )

        if saferatio(error_magnitude, threshold, 100):
            with builder.paragraph() as txt:
                txt += domain.gettext("errors_maybe_rounding")
                txt += domain.gettext("double_precision_hint")

    return builder.build()
